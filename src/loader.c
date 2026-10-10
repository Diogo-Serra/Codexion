/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 21:50:55 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 21:51:00 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	load_config(t_scheduler *c, char **argv)
{
	c->num_coder = ft_atoi(argv[1]);
	c->num_dongles = c->num_coder;
	c->time_to_burnout = ft_atoi(argv[2]);
	c->time_to_compile = ft_atoi(argv[3]);
	c->time_to_debug = ft_atoi(argv[4]);
	c->time_to_refactor = ft_atoi(argv[5]);
	c->num_compiles_required = ft_atoi(argv[6]);
	c->dongle_cooldown = ft_atoi(argv[7]);
	if (ft_strcmp(argv[8], "fifo") == 0)
		c->policy = FIFO;
	else if (ft_strcmp(argv[8], "edf") == 0)
		c->policy = EDF;
	else
		return (1);
	return (0);
}

static t_engine	*free_engine(t_engine *engine)
{
	free(engine->coders);
	free(engine->dongles);
	free(engine);
	return (NULL);
}

static t_engine	*alloc_engine(int n)
{
	t_engine	*engine;

	engine = malloc(sizeof(t_engine));
	if (!engine)
		return (NULL);
	engine->coders = malloc(n * sizeof(t_coder));
	engine->dongles = malloc(n * sizeof(t_dongle));
	if (!engine->coders || !engine->dongles)
		return (free_engine(engine));
	return (engine);
}

t_engine	*loader(char **argv)
{
	t_engine	*engine;
	int			n;

	n = ft_atoi(argv[1]);
	engine = alloc_engine(n);
	if (!engine)
		return (NULL);
	if (load_config(&engine->config, argv))
		return (free_engine(engine));
	engine->num_coders_alive = n;
	engine->total_compiles = 0;
	engine->simulation_end = false;
	engine->simulation_start = 0;
	init_dongles(engine->dongles, n);
	init_coders(engine, n);
	return (engine);
}
