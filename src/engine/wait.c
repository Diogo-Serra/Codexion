/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:55:20 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 22:01:31 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Wait for a dongle (caller holds dongle->lock, and holds it again on return)
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

// Wait on the dongle that was not ready, then release its lock
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

// Take both dongles and start the new deadline atomically with the take
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