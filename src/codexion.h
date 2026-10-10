/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:18:51 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 20:08:32 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

typedef enum e_policy
{
	FIFO,
	EDF
}	t_policy;

typedef struct s_dongle
{
	int		id;
	long	last_released;
	long	cooldown_until;
	bool	is_available;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				num_compiles;
	long			last_compile_start;
	long			last_compile_end;
	long			last_debug_start;
	long			last_refactor_start;
	long			burnout_time;
	bool			is_alive;
	t_dongle		*left;
	t_dongle		*right;
	struct s_engine	*engine;
}	t_coder;

typedef struct s_scheduler
{
	int			num_coder;
	int			num_dongles;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			num_compiles_required;
	int			dongle_cooldown;
	t_policy	policy;
}	t_scheduler;

typedef struct s_engine
{
	t_coder		*coders;
	t_dongle	*dongles;
	t_scheduler	config;
	long		simulation_start;
	bool		simulation_end;
	int			num_coders_alive;
	int			total_compiles;
}	t_engine;

t_engine	*loader(char **argv);
void		init_dongles(t_dongle *dongles, int n);
void		init_coders(t_engine *e, int n);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_atoi(const char *s);
const char	*get_arg(int index);
int			parser(int argc, char **argv);
int			is_valid_number(const char *s);

#endif