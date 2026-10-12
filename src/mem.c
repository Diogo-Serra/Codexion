/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 19:20:23 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 19:48:43 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	clear_scheduler(t_scheduler	*scheduler)
{
	if (scheduler->thread_s)
		free(scheduler->thread_s);
	if (scheduler->dongles)
		free(scheduler->dongles);
	if (scheduler->coders)
		free(scheduler->coders);
	return (0);
}

int	ft_exit(t_scheduler *scheduler, char *message)
{
	int	i;

	i = -1;
	while (++i < scheduler->coder_num)
	{
		pthread_mutex_destroy(&scheduler->dongles[i]);
		pthread_mutex_destroy(&scheduler->coders[i].lock);
	}
	pthread_mutex_destroy(&scheduler->write);
	pthread_mutex_destroy(&scheduler->lock);
	clear_scheduler(scheduler);
	if (message)
		printf("%s\n", message);
	return (0);
}
