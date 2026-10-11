/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 14:28:09 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 21:49:10 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_atoi(const char *s)
{
	int	number;
	int	sign;

	number = 0;
	sign = 1;
	while ((*s == 32) || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s++ == '-')
			sign *= -1;
	}
	while (*s >= '0' && *s <= '9')
		number = (number * 10) + (*s++ - '0');
	return (number * sign);
}

const char	*get_arg(int index)
{
	if (index == 1)
		return ("NUM_CODERS");
	else if (index == 2)
		return ("BURNOUT");
	else if (index == 3)
		return ("COMPILE");
	else if (index == 4)
		return ("DEBUG");
	else if (index == 5)
		return ("REFACTOR");
	else if (index == 6)
		return ("NUM_COMPILES");
	else if (index == 7)
		return ("DONGLE_COOLDOWN");
	else if (index == 8)
		return ("SCHEDULER");
	return (NULL);
}

int	is_valid_number(const char *s)
{
	if (!*s)
		return (0);
	while (*s)
		s++;
	return (1);
}

char	*get_enum(bool	n)
{
	if (n == 0)
		return ("fifo");
	if (n == 1)
		return ("edf");
	return (NULL);
}

void	show_settings(t_engine	*engine)
{
		printf("%i\n", engine->config.num_coder);
		printf("%i\n", engine->config.num_dongles);
		printf("%i\n", engine->config.time_to_burnout);
		printf("%i\n", engine->config.time_to_compile);
		printf("%i\n", engine->config.time_to_debug);
		printf("%i\n", engine->config.time_to_refactor);
		printf("%i\n", engine->config.num_compiles_required);
		printf("%i\n", engine->config.dongle_cooldown);
		printf("%s\n", get_enum(engine->config.policy));
}