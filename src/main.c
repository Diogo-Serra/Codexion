/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:57:09 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 18:02:35 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	int	i;
	int	operation;

	operation = parser(argc, argv);
	if (operation == 1)
		return (1);
	i = 1;
	printf("Data:\n");
	while (i <= argc - 1)
	{
		printf("%-18s%s\n", get_arg(i), argv[i]);
		i++;
	}
	if (operation == 2)
		return (0);
	if (operation == 3)
		return (0);
	return (0);
}
