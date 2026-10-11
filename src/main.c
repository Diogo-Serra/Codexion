/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:57:09 by diserra           #+#    #+#             */
/*   Updated: 2026/10/11 16:25:26 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	int			operation;
	t_engine	*engine;

	operation = parser(argc, argv);
	if (operation == 1)
		return (1);
	engine = loader(argv);
	if (operation == 2)
	{
		show_settings(engine);
	}
	if (operation == 3)
	{

		show_settings(engine);
	}
	return (0);
}
