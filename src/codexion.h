/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:18:51 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 21:58:04 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <pthread.h>
# include <unistd.h>

//	parsing_errors
# define ERROR_ARGS "INVALID NUMBER OF ARGUMENTS"
# define ERROR_INT "ARGUMENTS NEED TO BE POSITIVE INTS"
# define ERROR_SCHEDULER "<SCHEDULER> NEEDS TO BE fifo OR edf"
//	alloc_errors
# define ALLOC_ERROR_2 "ERROR ALLOCATING DONGLES"
# define ALLOC_ERROR_3 "ERROR ALLOCATING CODERS"
//	pthread_errors
# define THREAD_ERROR "ERROR WHILE CREATING THREADS"
//	time_error
# define TIME_ERROR "UNABLE TO RETRIVE UTC"
//	monitor_states
# define ST_BURNED 1
# define ST_DONE 2
//	acquire_states
# define ACQ_WAIT 0
# define ACQ_TAKEN 1
# define ACQ_STOP 2
//	dongle queue
# define QUEUE_MAX 2
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

typedef struct s_request
{
	u_int64_t	key;
	int			id;
}	t_request;

typedef struct s_dongle
{
	bool			taken;
	int				size;
	u_int64_t		available_at;
	t_request		heap[QUEUE_MAX];
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	int					num_compiles;
	u_int64_t			burnout_time;
	pthread_t			thread_c;
	pthread_mutex_t		lock;
	t_dongle			*dongle_l;
	t_dongle			*dongle_r;
	struct s_scheduler	*scheduler;
}	t_coder;

typedef struct s_scheduler
{
	int				coder_num;
	int				num_compiles;
	bool			is_alive;
	t_coder			*coders;
	u_int64_t		dongle_cooldown;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
	u_int64_t		burnout_time;
	u_int64_t		start_time;
	u_int64_t		ticket;
	pthread_t		monitor_t;
	t_dongle		*dongles;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
	t_policy		policy;
}	t_scheduler;

int			ft_atoi(const char *s);
const char	*get_arg(int index);
int			parser(int argc, char **argv);
u_int64_t	get_time(void);
int			initialize(t_scheduler *scheduler, char **argv, t_policy flag);
int			ft_exit(t_scheduler *scheduler, char *message);
void		free_scheduler(t_scheduler *scheduler);
int			is_running(t_scheduler *scheduler);
void		log_state(t_coder *coder, const char *message);
void		log_burnout(t_coder *coder);
void		stop_simulation(t_scheduler *scheduler);
void		smart_sleep(t_scheduler *scheduler, u_int64_t duration);
void		*monitor(void *pointer);
void		*routine(void *pointer);
int			run_simulation(t_scheduler *scheduler);
void		heap_push(t_dongle *dongle, u_int64_t key, int id);
void		heap_pop(t_dongle *dongle);
int			is_ready(t_coder *coder, t_dongle *dongle);
void		wait_on(t_dongle *dongle, t_coder *coder);
void		block_on(t_coder *coder, t_dongle *a, t_dongle *b);
void		take_both(t_dongle *a, t_dongle *b);
int			acquire_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
void		wake_dongles(t_scheduler *scheduler);

#endif
