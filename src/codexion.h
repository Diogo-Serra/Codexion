/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:18:51 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 19:47:43 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>

//	parsing_errors
# define ERROR_ARGS "INVALID NUMBER OF ARGUMENTS"
# define ERROR_INT "ARGUMENTS NEED TO BE POSITIVE INTS"
# define ERROR_SCHEDULER "<SCHEDULER> NEEDS TO BE fifo OR edf"
//	alloc_errors
# define ALLOC_ERROR_1 "ERROR ALLOCATING THREADS"
# define ALLOC_ERROR_2 "ERROR ALLOCATING DONGLES"
# define ALLOC_ERROR_3 "ERROR ALLOCATING CODERS"
//	pthread_errors
# define SCHEDULER_ERROR "ERROR WHILE LOADING SCHEDULER"
# define THREAD_ERROR "ERROR WHILE CREATING THREADS"
# define ALLOC_ERROR "ERROR WHILE JOINING THREADS"
# define INIT_DONGLES "ERROR WHILE INIT DONGLES"
# define INIT_CODERS "ERROR WHILE INIT CODERS"
//	time_error
# define TIME_ERROR "UNABLE TO RETRIVE UTC"
//	coder_messages
# define TAKE_DONGLE "has taken a dongle"
# define COMPILING "is compiling"
# define DEBUGGING "is debugging"
# define REFACTORIG "is refactoring"
# define BURNOUT "burned out"

typedef enum e_policy
{
	FIFO,
	EDF
}	t_policy;

typedef struct s_coder
{
	int					id;
	int					num_compiles;
	bool				compiling;
	bool				is_alive;
	u_int64_t			burnout_time;
	pthread_t			thread_c;
	pthread_mutex_t		lock;
	pthread_mutex_t		*dongle_l;
	pthread_mutex_t		*dongle_r;
	struct s_scheduler	*scheduler;
}	t_coder;

typedef struct s_scheduler
{
	pthread_t		*thread_s;
	int				coder_num;
	int				num_compiles;
	bool			is_alive;
	bool			is_finished;
	t_coder			*coders;
	u_int64_t		dongle_cooldown;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
	u_int64_t		burnout_time;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
	t_policy		policy;
}	t_scheduler;

int			ft_atoi(const char *s);
const char	*get_arg(int index);
int			parser(int argc, char **argv);
u_int64_t	get_time(void);
t_scheduler	initialize(char **argv, t_policy flag);
int			ft_exit(t_scheduler *scheduler, char *message);

#endif