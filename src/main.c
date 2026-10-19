/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare<diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:57:09 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/12 21:58:23 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Entry point of the simulation
int	main(int argc, char **argv)
{
	int			operation;
	int			status;
	t_policy	policy;
	t_scheduler	scheduler;

	operation = parser(argc, argv);
	if (operation == 1)
		return (1);
	policy = FIFO;
	if (operation == 2)
		policy = EDF;
	if (initialize(&scheduler, argv, policy))
		return (1);
	status = run_simulation(&scheduler);
	ft_exit(&scheduler);
	return (status);
}
