/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:10:27 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:10:29 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Parse args, run the simulation, then always clean up. */
int	main(int argc, char **argv)
{
	t_scheduler	scheduler;
	t_policy	policy;
	int			flag;

	flag = parser(argc, argv);
	if (flag == 2)
		policy = EDF;
	else if (flag == 3)
		policy = FIFO;
	else
		return (1);
	if (initialize(&scheduler, argv, policy))
		return (1);
	if (run_simulation(&scheduler))
	{
		ft_exit(&scheduler);
		return (1);
	}
	ft_exit(&scheduler);
	return (0);
}
