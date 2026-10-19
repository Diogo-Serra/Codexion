/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 19:20:23 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 21:59:12 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Clean up scheduler memory
void	free_scheduler(t_scheduler *scheduler)
{
	int	i;

	if (scheduler->dongles)
	{
		i = -1;
		while (++i < scheduler->coder_num)
			free(scheduler->dongles[i].heap);
	}
	free(scheduler->dongles);
	free(scheduler->coders);
}

// Destroy synchronization primitives and free memory
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
