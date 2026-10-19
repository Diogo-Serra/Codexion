/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:10:10 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:10:12 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Free the two heap arrays only; dongle->heap is inline, never free it. */
void	free_scheduler(t_scheduler *scheduler)
{
	free(scheduler->dongles);
	free(scheduler->coders);
	scheduler->dongles = NULL;
	scheduler->coders = NULL;
}

/* Destroy every mutex/cond, then release the heap arrays. */
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
