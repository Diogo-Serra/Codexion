/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:09:08 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:09:11 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* FIFO uses a ticket counter; EDF uses the coder's current deadline. */
static u_int64_t	request_key(t_coder *coder)
{
	t_scheduler	*scheduler;
	u_int64_t	key;

	scheduler = coder->scheduler;
	if (scheduler->policy == EDF)
	{
		pthread_mutex_lock(&coder->lock);
		key = coder->burnout_time;
		pthread_mutex_unlock(&coder->lock);
		return (key);
	}
	pthread_mutex_lock(&scheduler->lock);
	key = scheduler->ticket++;
	pthread_mutex_unlock(&scheduler->lock);
	return (key);
}

/* Push the coder onto both dongle heaps under both locks. */
static void	enqueue_both(t_coder *coder, t_dongle *a, t_dongle *b)
{
	u_int64_t	key;

	pthread_mutex_lock(&a->lock);
	pthread_mutex_lock(&b->lock);
	key = request_key(coder);
	heap_push(a, key, coder->id);
	heap_push(b, key, coder->id);
	pthread_mutex_unlock(&b->lock);
	pthread_mutex_unlock(&a->lock);
}

/* Try to take both dongles if we are head of both queues. */
static int	attempt(t_coder *coder, t_dongle *a, t_dongle *b)
{
	int	state;
	int	ready_a;

	pthread_mutex_lock(&a->lock);
	pthread_mutex_lock(&b->lock);
	state = ACQ_WAIT;
	ready_a = is_ready(coder, a);
	if (!is_running(coder->scheduler))
		state = ACQ_STOP;
	else if (ready_a && is_ready(coder, b))
		state = ACQ_TAKEN;
	if (state == ACQ_TAKEN)
		take_both(coder, a, b);
	if (state == ACQ_WAIT)
		block_on(coder, a, b, ready_a);
	else
	{
		pthread_mutex_unlock(&b->lock);
		pthread_mutex_unlock(&a->lock);
	}
	return (state);
}

/* Single-dongle case: sleep past the burnout deadline and return 1. */
static int	single_dongle(t_coder *coder)
{
	smart_sleep(coder->scheduler, coder->scheduler->burnout_time + 100);
	return (1);
}

/* Enqueue, wait until both are ours, log the take; return 0 or 1. */
int	acquire_dongles(t_coder *coder)
{
	t_dongle	*a;
	t_dongle	*b;
	int			state;

	if (coder->dongle_l == coder->dongle_r)
		return (single_dongle(coder));
	a = coder->dongle_l;
	b = coder->dongle_r;
	if (a > b)
	{
		a = coder->dongle_r;
		b = coder->dongle_l;
	}
	enqueue_both(coder, a, b);
	state = ACQ_WAIT;
	while (state == ACQ_WAIT)
		state = attempt(coder, a, b);
	if (state == ACQ_STOP)
		return (1);
	log_acquire(coder);
	return (0);
}
