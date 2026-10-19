/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 16:40:26 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 21:58:53 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Allocate memory for scheduler resources
static int	alloc_init(t_scheduler *scheduler)
{
	scheduler->dongles = malloc(scheduler->coder_num * sizeof(t_dongle));
	if (!scheduler->dongles)
	{
		printf("%s\n", ALLOC_ERROR_2);
		return (1);
	}
	scheduler->coders = malloc(scheduler->coder_num * sizeof(t_coder));
	if (!scheduler->coders)
	{
		printf("%s\n", ALLOC_ERROR_3);
		return (1);
	}
	return (0);
}

// Initialize dongle synchronization primitives
static void	init_dongles(t_scheduler *scheduler)
{
	int	i;
	int	n;

	n = scheduler->coder_num;
	i = -1;
	while (++i < n)
	{
		memset(&scheduler->dongles[i], 0, sizeof(t_dongle));
		pthread_mutex_init(&scheduler->dongles[i].lock, NULL);
		pthread_cond_init(&scheduler->dongles[i].cond, NULL);
	}
	i = -1;
	while (++i < n)
	{
		scheduler->coders[i].dongle_r = &scheduler->dongles[i];
		scheduler->coders[i].dongle_l = &scheduler->dongles[(i + n - 1) % n];
	}
}

// Initialize coder properties and locks
static void	init_coders(t_scheduler *scheduler)
{
	int	i;

	i = -1;
	while (++i < scheduler->coder_num)
	{
		scheduler->coders[i].scheduler = scheduler;
		scheduler->coders[i].id = i + 1;
		scheduler->coders[i].num_compiles = 0;
		scheduler->coders[i].is_compiling = false;
		scheduler->coders[i].finished = false;
		pthread_mutex_init(&scheduler->coders[i].lock, NULL);
	}
}

// Load configuration from command line arguments
static void	load_scheduler(t_scheduler *scheduler, char **argv, t_policy flag)
{
	scheduler->coder_num = ft_atoi(argv[1]);
	scheduler->burnout_time = (u_int64_t)ft_atoi(argv[2]);
	scheduler->compile_time = (u_int64_t)ft_atoi(argv[3]);
	scheduler->debug_time = (u_int64_t)ft_atoi(argv[4]);
	scheduler->refactor_time = (u_int64_t)ft_atoi(argv[5]);
	scheduler->num_compiles = ft_atoi(argv[6]);
	scheduler->dongle_cooldown = (u_int64_t)ft_atoi(argv[7]);
	scheduler->policy = flag;
	scheduler->is_alive = true;
}

// Perform full system initialization
int	initialize(t_scheduler *scheduler, char **argv, t_policy flag)
{
	memset(scheduler, 0, sizeof(t_scheduler));
	load_scheduler(scheduler, argv, flag);
	if (alloc_init(scheduler))
	{
		free_scheduler(scheduler);
		return (1);
	}
	init_dongles(scheduler);
	init_coders(scheduler);
	pthread_mutex_init(&scheduler->write, NULL);
	pthread_mutex_init(&scheduler->lock, NULL);
	return (0);
}
