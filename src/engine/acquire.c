/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:55:38 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 22:01:57 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Determine priority key based on scheduling policy
static u_int64_t	request_key(t_coder *coder)
{
	t_scheduler	*scheduler;
	u_int64_t	key;

	scheduler = coder->scheduler;
	if (scheduler->policy == EDF)
		return (coder->burnout_time);
	pthread_mutex_lock(&scheduler->lock);
	key = scheduler->ticket++;
	pthread_mutex_unlock(&scheduler->lock);
	return (key);
}

// Add coder to the priority queues of both required dongles
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

// Try to acquire both dongles if the coder is at the head of both queues
static int	attempt(t_coder *coder, t_dongle *a, t_dongle *b)
{
	int	state;

	pthread_mutex_lock(&a->lock);
	pthread_mutex_lock(&b->lock);
	state = ACQ_WAIT;
	if (!is_running(coder->scheduler))
		state = ACQ_STOP;
	else if (is_ready(coder, a) && is_ready(coder, b))
		state = ACQ_TAKEN;
	if (state == ACQ_TAKEN)
		take_both(a, b);
	if (state == ACQ_WAIT)
		block_on(coder, a, b);
	else
	{
		pthread_mutex_unlock(&b->lock);
		pthread_mutex_unlock(&a->lock);
	}
	return (state);
}

// A lone coder cannot hold two dongles and will burn out
static int	single_dongle(t_coder *coder)
{
	while (is_running(coder->scheduler))
		usleep(500);
	return (1);
}

// Main for acquiring required dongles
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
	log_state(coder, TAKE_DONGLE);
	log_state(coder, TAKE_DONGLE);
	return (0);
}
