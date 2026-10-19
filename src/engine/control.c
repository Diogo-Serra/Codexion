/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 15:00:00 by diserra           #+#    #+#             */
/*   Updated: 2026/10/19 15:00:00 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop_simulation(t_scheduler *scheduler)
{
	pthread_mutex_lock(&scheduler->lock);
	scheduler->is_alive = false;
	pthread_mutex_unlock(&scheduler->lock);
	wake_dongles(scheduler);
}

/* Sleep with microsecond precision while checking the simulation is alive */
void	smart_sleep(t_scheduler *scheduler, u_int64_t duration)
{
	u_int64_t	end;
	u_int64_t	now;

	end = get_time_us() + (duration * 1000);
	now = get_time_us();
	while (now < end && is_running(scheduler))
	{
		if (end - now > 2000)
			usleep(1000);
		else
			usleep(200);
		now = get_time_us();
	}
}

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
