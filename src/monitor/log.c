/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:09:45 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:09:46 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Thread-safe read of the scheduler's is_alive flag. */
int	is_running(t_scheduler *scheduler)
{
	int	running;

	pthread_mutex_lock(&scheduler->lock);
	running = scheduler->is_alive;
	pthread_mutex_unlock(&scheduler->lock);
	return (running);
}

/* Print one timestamped state line under the write lock, if alive. */
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

/* Print the 3-line TAKE/TAKE/COMPILING group atomically under one lock. */
void	log_acquire(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = coder->scheduler;
	pthread_mutex_lock(&scheduler->write);
	if (is_running(scheduler))
	{
		printf("%llu %d %s\n",
			(unsigned long long)(get_time() - scheduler->start_time),
			coder->id, TAKE_DONGLE);
		printf("%llu %d %s\n",
			(unsigned long long)(get_time() - scheduler->start_time),
			coder->id, TAKE_DONGLE);
		printf("%llu %d %s\n",
			(unsigned long long)(get_time() - scheduler->start_time),
			coder->id, COMPILING);
		fflush(stdout);
	}
	pthread_mutex_unlock(&scheduler->write);
}

/* Mark dead, print the burnout line, flush, then wake all waiters. */
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
	fflush(stdout);
	pthread_mutex_unlock(&scheduler->write);
	wake_dongles(scheduler);
}
