/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:56:07 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 22:02:11 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Release a dongle and update its availability
static void	release_one(t_dongle *dongle, u_int64_t cooldown)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->taken = false;
	dongle->available_at = get_time() + cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

// Release both dongles held by a coder
void	release_dongles(t_coder *coder)
{
	release_one(coder->dongle_l, coder->scheduler->dongle_cooldown);
	release_one(coder->dongle_r, coder->scheduler->dongle_cooldown);
}

// Wake up all coders waiting on any dongle
void	wake_dongles(t_scheduler *scheduler)
{
	int	i;

	i = -1;
	while (++i < scheduler->coder_num)
	{
		pthread_mutex_lock(&scheduler->dongles[i].lock);
		pthread_cond_broadcast(&scheduler->dongles[i].cond);
		pthread_mutex_unlock(&scheduler->dongles[i].lock);
	}
}
