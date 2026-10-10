/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:57:09 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 21:45:28 by diserra          ###   ########.fr       */
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
	if (operation == 2)
	{
		engine = loader(argv);
		printf("%i\n", engine->config.num_coder);
		printf("%i\n", engine->config.num_dongles);
		printf("%i\n", engine->config.time_to_burnout);
		printf("%i\n", engine->config.time_to_compile);
		printf("%i\n", engine->config.time_to_debug);
		printf("%i\n", engine->config.time_to_refactor);
		printf("%i\n", engine->config.num_compiles_required);
		printf("%i\n", engine->config.dongle_cooldown);
		printf("%i\n", engine->config.policy);
	}
	if (operation == 3)
		return (0);
	return (0);
}
