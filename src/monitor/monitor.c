/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:09:50 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:09:51 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Classify one coder as DONE, BURNED, or still running (0). */
static int	check_coder(t_coder *coder)
{
	int	state;

	state = 0;
	pthread_mutex_lock(&coder->lock);
	if (coder->finished)
		state = ST_DONE;
	else if (!coder->is_compiling && get_time() >= coder->burnout_time)
		state = ST_BURNED;
	pthread_mutex_unlock(&coder->lock);
	return (state);
}

/* Return -1 on any burnout, otherwise the number of coders done. */
static int	scan_coders(t_scheduler *scheduler)
{
	int	i;
	int	done;
	int	state;

	i = -1;
	done = 0;
	while (++i < scheduler->coder_num)
	{
		state = check_coder(&scheduler->coders[i]);
		if (state == ST_BURNED)
		{
			log_burnout(&scheduler->coders[i]);
			return (-1);
		}
		done += (state == ST_DONE);
	}
	return (done);
}

/* Poll every coder; stop the simulation on burnout or all-done. */
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
