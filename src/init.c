/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 16:40:26 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 20:16:52 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	alloc_init(t_scheduler *scheduler)
{
	scheduler->thread_s = malloc(scheduler->coder_num * sizeof(pthread_t));
	if (!scheduler->thread_s)
	{
		printf("%s\n", ALLOC_ERROR_1);
		return (1);
	}
	scheduler->dongles = malloc(scheduler->coder_num * sizeof(pthread_mutex_t));
	if (!scheduler->dongles)
	{
		printf("%s\n", ALLOC_ERROR_2);
		return (1);
	}
	scheduler->coders = malloc(scheduler->coder_num * sizeof(t_coder));
	if (!scheduler->coders) {
		printf("%s\n", ALLOC_ERROR_3);
		return (1);
	}
	return (0);
}

static int	init_dongles(t_scheduler *scheduler)
{
	int	i;

	i = -1;
	while (++i < scheduler->coder_num)
		pthread_mutex_init(&scheduler->dongles[i], NULL);
	i = 0;
	scheduler->coders[i].dongle_l = &scheduler->dongles[i];
	scheduler->coders[i].dongle_r = &scheduler->dongles[i - 1];
	i = 1;
	while (i < scheduler->coder_num)
	{
		scheduler->coders[i].dongle_l = &scheduler->dongles[i];
		scheduler->coders[i].dongle_r = &scheduler->dongles[i - 1];
		i++;
	}
	return (0);
}

static int	init_coders(t_scheduler *scheduler)
{
	int	i;

	i = 0;
	while (i < scheduler->coder_num)
	{
		scheduler->coders[i].scheduler = scheduler;
		scheduler->coders[i].id = i + 1;
		scheduler->coders[i].burnout_time = scheduler->burnout_time;
		scheduler->coders[i].num_compiles = 0;
		scheduler->coders[i].compiling = false;
		scheduler->coders[i].is_alive = true;
		pthread_mutex_init(&scheduler->coders[i].lock, NULL);
		i++;
	}
	return 0;
}

static int	load_scheduler(t_scheduler *scheduler, char **argv, t_policy flag)
{
	scheduler->coder_num = ft_atoi(argv[1]);
	scheduler->burnout_time = (u_int64_t) ft_atoi(argv[2]);
	scheduler->compile_time = (u_int64_t) ft_atoi(argv[3]);
	scheduler->debug_time = (u_int64_t) ft_atoi(argv[4]);
	scheduler->refactor_time = (u_int64_t) ft_atoi(argv[5]);
	scheduler->num_compiles = ft_atoi(argv[6]);
	scheduler->dongle_cooldown = ft_atoi(argv[7]);
	scheduler->policy = flag;
	scheduler->is_alive = true;
	scheduler->is_finished = false;
	pthread_mutex_init(&scheduler->write, NULL);
	pthread_mutex_init(&scheduler->lock, NULL);
	return (0);
}

t_scheduler	initialize(char **argv, t_policy flag)
{
	t_scheduler		scheduler;

	if (load_scheduler(&scheduler, argv, flag))
		return (ft_exit(&scheduler, SCHEDULER_ERROR), scheduler);
	if (alloc_init(&scheduler))
		return (ft_exit(&scheduler, ALLOC_ERROR), scheduler);
	if (init_dongles(&scheduler))
		return (ft_exit(&scheduler, INIT_DONGLES), scheduler);
	if (init_coders(&scheduler))
		return (ft_exit(&scheduler, INIT_CODERS), scheduler);
	return (scheduler);
}
