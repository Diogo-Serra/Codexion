/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:52:13 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 17:56:53 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parser(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 9)
	{
		printf("Usage: codexion <num_coders> <burnout> <compile> <debug>");
		printf(" <refactor> <compiles> <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	while (i <= 7)
	{
		if (!ft_atoi(argv[i]) || ft_atoi(argv[i]) < 0)
		{
			printf("<%s>: It needs to be a positive int\n", get_arg(i));
			return (1);
		}
		i++;
	}
	if (!ft_strcmp(argv[8], "edf"))
		return (2);
	else if (!ft_strcmp(argv[8], "fifo"))
		return (3);
	printf("<SCHEDULER> needs to be fifo or edf\n");
	return (1);
}
