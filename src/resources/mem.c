/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 19:20:23 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 22:30:00 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_scheduler(t_scheduler *scheduler)
{
	free(scheduler->dongles);
	free(scheduler->coders);
	scheduler->dongles = NULL;
	scheduler->coders = NULL;
}

void	ft_exit(t_scheduler *scheduler)
{
	int	i;

	i = -1;
	while (++i < scheduler->coder_num)
	{
		pthread_mutex_destroy(&scheduler->dongles[i].lock);
		pthread_cond_destroy(&scheduler->dongles[i].cond);
		pthread_mutex_destroy(&scheduler->coders[i].lock);
	}
	pthread_mutex_destroy(&scheduler->write);
	pthread_mutex_destroy(&scheduler->lock);
	free_scheduler(scheduler);
}
