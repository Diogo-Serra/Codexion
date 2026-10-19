/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:10:04 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:10:05 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Return zero if both strings are equal, non-zero otherwise. */
static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* Accept only an optional '+' followed by digits that fit in int. */
static int	is_valid_number(const char *s)
{
	long	value;

	if (*s == '+')
		s++;
	if (!*s)
		return (0);
	value = 0;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		value = (value * 10) + (*s++ - '0');
		if (value > INT_MAX)
			return (0);
	}
	return (1);
}

/* Validate all seven numbers; num_coders and num_compiles must be >= 1. */
static int	check_numbers(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!is_valid_number(argv[i])
			|| ((i == 1 || i == 6) && ft_atoi(argv[i]) < 1))
		{
			printf("<%s>: %s\n", get_arg(i), ERROR_INT);
			return (1);
		}
		i++;
	}
	return (0);
}

/* Validate argv; return 2 for edf, 3 for fifo, 1 on error. */
int	parser(int argc, char **argv)
{
	if (argc != 9)
	{
		printf("USAGE: codexion <num_coders> <burnout> <compile> <debug>");
		printf(" <refactor> <compiles> <dongle_cooldown> <scheduler>\n");
		printf("%s\n", ERROR_ARGS);
		return (1);
	}
	if (check_numbers(argv))
		return (1);
	if (!ft_strcmp(argv[8], "edf"))
		return (2);
	else if (!ft_strcmp(argv[8], "fifo"))
		return (3);
	printf("%s\n", ERROR_SCHEDULER);
	return (1);
}
