/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 21:28:59 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 22:00:11 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Check if a coder has burned out or finished their tasks
static int	check_coder(t_coder *coder, int required)
{
	int	state;

	state = 0;
	pthread_mutex_lock(&coder->lock);
	if (get_time() >= coder->burnout_time)
		state = ST_BURNED;
	else if (coder->num_compiles >= required)
		state = ST_DONE;
	pthread_mutex_unlock(&coder->lock);
	return (state);
}

// Scan all coders to check for burnout or completion
static int	scan_coders(t_scheduler *scheduler)
{
	int	i;
	int	done;
	int	state;

	i = -1;
	done = 0;
	while (++i < scheduler->coder_num)
	{
		state = check_coder(&scheduler->coders[i], scheduler->num_compiles);
		if (state == ST_BURNED)
		{
			log_burnout(&scheduler->coders[i]);
			return (-1);
		}
		done += (state == ST_DONE);
	}
	return (done);
}

// Monitor thread routine that checks simulation status
void	*monitor(void *pointer)
{
	t_scheduler	*scheduler;
	int			done;

	scheduler = (t_scheduler *)pointer;
	while (is_running(scheduler))
	{
		done = scan_coders(scheduler);
		if (done < 0)
			break ;
		if (done == scheduler->coder_num)
			stop_simulation(scheduler);
		usleep(1000);
	}
	return (NULL);
}
