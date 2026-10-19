/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:09:30 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:09:32 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Sleep for the compile duration, then bump and clear is_compiling. */
static void	compile(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = coder->scheduler;
	smart_sleep(scheduler, scheduler->compile_time);
	pthread_mutex_lock(&coder->lock);
	coder->num_compiles++;
	coder->is_compiling = false;
	pthread_mutex_unlock(&coder->lock);
}

/* Return 1 and mark the coder finished once num_compiles reaches required. */
static int	should_stop(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->lock);
	done = coder->num_compiles >= coder->scheduler->num_compiles;
	if (done)
		coder->finished = true;
	pthread_mutex_unlock(&coder->lock);
	return (done);
}

/* Run one full compile + debug + refactor cycle for a coder. */
static void	work_cycle(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = coder->scheduler;
	compile(coder);
	release_dongles(coder);
	log_state(coder, DEBUGGING);
	smart_sleep(scheduler, scheduler->debug_time);
	log_state(coder, REFACTORING);
	smart_sleep(scheduler, scheduler->refactor_time);
}

/* Coder thread: loop until done or simulation stops, one cycle per turn. */
void	*routine(void *pointer)
{
	t_coder		*coder;
	t_scheduler	*scheduler;

	coder = (t_coder *)pointer;
	scheduler = coder->scheduler;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (is_running(scheduler))
	{
		if (should_stop(coder))
			break ;
		if (acquire_dongles(coder))
			break ;
		work_cycle(coder);
	}
	return (NULL);
}
