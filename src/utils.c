/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 14:28:09 by diserra           #+#    #+#             */
/*   Updated: 2026/10/10 17:17:42 by diserra          ###   ########.fr       */
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