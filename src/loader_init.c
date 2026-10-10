/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 21:50:06 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 21:50:34 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongles(t_dongle *dongles, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		dongles[i].id = i;
		dongles[i].last_released = 0;
		dongles[i].cooldown_until = 0;
		dongles[i].is_available = true;
		i++;
	}
}

void	init_coders(t_engine *e, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		e->coders[i].id = i + 1;
		e->coders[i].num_compiles = 0;
		e->coders[i].last_compile_start = 0;
		e->coders[i].last_compile_end = 0;
		e->coders[i].last_debug_start = 0;
		e->coders[i].last_refactor_start = 0;
		e->coders[i].burnout_time = 0;
		e->coders[i].is_alive = true;
		e->coders[i].left = &e->dongles[i];
		e->coders[i].right = &e->dongles[(i + 1) % n];
		e->coders[i].engine = e;
		i++;
	}
}
