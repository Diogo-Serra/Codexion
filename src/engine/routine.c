/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:29:21 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 22:23:38 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Simulate the compilation process
static void	compile(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = coder->scheduler;
	pthread_mutex_lock(&coder->lock);
	coder->burnout_time = get_time() + scheduler->burnout_time;
	coder->is_compiling = true;
	pthread_mutex_unlock(&coder->lock);
	log_state(coder, COMPILING);
	smart_sleep(scheduler, scheduler->compile_time);
	pthread_mutex_lock(&coder->lock);
	coder->num_compiles++;
	coder->is_compiling = false;
	pthread_mutex_unlock(&coder->lock);
}

// Thread routine for each coder simulating their work cycle
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
		if (coder->num_compiles >= scheduler->num_compiles)
			break ;
		if (acquire_dongles(coder))
			break ;
		compile(coder);
		release_dongles(coder);
		log_state(coder, DEBUGGING);
		smart_sleep(scheduler, scheduler->debug_time);
		log_state(coder, REFACTORIG);
		smart_sleep(scheduler, scheduler->refactor_time);
	}
	return (NULL);
}

// Stop simulation and join threads after a failure
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

// Initialize and start all coder and monitor threads
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
