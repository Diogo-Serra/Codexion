/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:55:20 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 22:01:31 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Wait for a dongle to become available
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
	pthread_mutex_unlock(&dongle->lock);
}

// Decide which dongle to wait on
void	block_on(t_coder *coder, t_dongle *a, t_dongle *b)
{
	if (!is_ready(coder, a))
	{
		pthread_mutex_unlock(&b->lock);
		wait_on(a, coder);
	}
	else
	{
		pthread_mutex_unlock(&a->lock);
		wait_on(b, coder);
	}
}

// Mark dongles as taken and remove coder from queues
void	take_both(t_dongle *a, t_dongle *b)
{
	a->taken = true;
	b->taken = true;
	heap_pop(a);
	heap_pop(b);
}
