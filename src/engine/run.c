/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:09:37 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:09:39 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Block on a dongle cond until ready or woken; reacquires its lock. */
void	wait_on(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;

	if (dongle->size > 0 && dongle->heap[0].id == coder->id && !dongle->taken)
	{
		ts.tv_sec = dongle->available_at / 1000;
		ts.tv_nsec = (dongle->available_at % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->cond, &dongle->lock, &ts);
	}
	else
		pthread_cond_wait(&dongle->cond, &dongle->lock);
}

/* Unlock the non-ready dongle, wait on the other, then unlock it. */
void	block_on(t_coder *coder, t_dongle *a, t_dongle *b, int ready_a)
{
	t_dongle	*target;

	if (!ready_a)
	{
		pthread_mutex_unlock(&b->lock);
		target = a;
	}
	else
	{
		pthread_mutex_unlock(&a->lock);
		target = b;
	}
	wait_on(target, coder);
	pthread_mutex_unlock(&target->lock);
}

/* Take both dongles at once and set the coder's new compile deadline. */
void	take_both(t_coder *coder, t_dongle *a, t_dongle *b)
{
	a->taken = true;
	b->taken = true;
	heap_pop(a);
	heap_pop(b);
	pthread_mutex_lock(&coder->lock);
	coder->burnout_time = get_time() + coder->scheduler->burnout_time;
	coder->is_compiling = true;
	pthread_mutex_unlock(&coder->lock);
}

/* Join any already-created coder threads and report the failure. */
static int	abort_start(t_scheduler *scheduler, int created)
{
	int	i;

	stop_simulation(scheduler);
	i = -1;
	while (++i < created)
		pthread_join(scheduler->coders[i].thread_c, NULL);
	printf("%s\n", THREAD_ERROR);
	return (1);
}

/* Spawn all coder threads plus the monitor, then join everything. */
int	run_simulation(t_scheduler *scheduler)
{
	int		i;
	t_coder	*coders;

	coders = scheduler->coders;
	scheduler->start_time = get_time();
	i = -1;
	while (++i < scheduler->coder_num)
	{
		coders[i].burnout_time = scheduler->start_time
			+ scheduler->burnout_time;
		if (pthread_create(&coders[i].thread_c, NULL, &routine, &coders[i]))
			return (abort_start(scheduler, i));
	}
	if (pthread_create(&scheduler->monitor_t, NULL, &monitor, scheduler))
		return (abort_start(scheduler, scheduler->coder_num));
	pthread_join(scheduler->monitor_t, NULL);
	i = -1;
	while (++i < scheduler->coder_num)
		pthread_join(coders[i].thread_c, NULL);
	return (0);
}
