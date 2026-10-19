/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:28:06 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 21:59:43 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Check if the simulation is still active
int	is_running(t_scheduler *scheduler)
{
	int	running;

	pthread_mutex_lock(&scheduler->lock);
	running = scheduler->is_alive;
	pthread_mutex_unlock(&scheduler->lock);
	return (running);
}

// Log a coder's state change to stdout
void	log_state(t_coder *coder, const char *message)
{
	t_scheduler	*scheduler;

	scheduler = coder->scheduler;
	pthread_mutex_lock(&scheduler->write);
	if (is_running(scheduler))
		printf("%llu %d %s\n",
			(unsigned long long)(get_time() - scheduler->start_time),
			coder->id, message);
	pthread_mutex_unlock(&scheduler->write);
}

// Log coder burnout and stop the simulation
void	log_burnout(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = coder->scheduler;
	pthread_mutex_lock(&scheduler->write);
	pthread_mutex_lock(&scheduler->lock);
	scheduler->is_alive = false;
	pthread_mutex_unlock(&scheduler->lock);
	printf("%llu %d %s\n",
		(unsigned long long)(get_time() - scheduler->start_time),
		coder->id, BURNOUT);
	pthread_mutex_unlock(&scheduler->write);
	wake_dongles(scheduler);
}

// Stop the simulation and wake all waiting threads
void	stop_simulation(t_scheduler *scheduler)
{
	pthread_mutex_lock(&scheduler->lock);
	scheduler->is_alive = false;
	pthread_mutex_unlock(&scheduler->lock);
	wake_dongles(scheduler);
}

// Sleeps while checking if simulation is still running
void	smart_sleep(t_scheduler *scheduler, u_int64_t duration)
{
	u_int64_t	end;

	end = get_time() + duration;
	while (get_time() < end && is_running(scheduler))
		usleep(500);
}
