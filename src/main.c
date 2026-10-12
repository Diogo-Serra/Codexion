/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:57:09 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 20:19:26 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	int			operation;
	t_scheduler	scheduler;
	
	operation = parser(argc, argv);
	// 1 - Fail , 2 - fifo, 3 - edf
	if (operation == 1)
		return (1);
	if (operation == 2)
	{
		scheduler = initialize(argv, EDF);
		ft_exit(&scheduler, "SUCCESS");
	}
	if (operation == 3)
	{
		scheduler = initialize(argv, FIFO);
		ft_exit(&scheduler, "SUCCESS");
	}
	return (0);
}
