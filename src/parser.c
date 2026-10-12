/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 17:52:13 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 18:33:38 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static int	is_valid_number(const char *s)
{
	if (!*s)
		return (0);
	while (*s)
		s++;
	return (1);
}

int	parser(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 9)
	{
		printf("USAGE: codexion <num_coders> <burnout> <compile> <debug>");
		printf(" <refactor> <compiles> <dongle_cooldown> <scheduler>\n");
		printf("%s\n", ERROR_ARGS);
		return (1);
	}
	while (i <= 7)
	{
		if (!is_valid_number(argv[i]) || ft_atoi(argv[i]) < 0)
		{
			printf("<%s>: %s\n", get_arg(i), ERROR_INT);
			return (1);
		}
		i++;
	}
	if (!ft_strcmp(argv[8], "edf"))
		return (2);
	else if (!ft_strcmp(argv[8], "fifo"))
		return (3);
	printf("%s\n", ERROR_SCHEDULER);
	return (1);
}
