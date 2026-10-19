/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosoare <diosoare@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 18:10:17 by diosoare          #+#    #+#             */
/*   Updated: 2026/10/19 18:10:18 by diosoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Parse a signed decimal integer from a null-terminated string. */
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

/* Map a 1-based argument index to its display name. */
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

/* Current wall-clock time in milliseconds since the Unix epoch. */
u_int64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		printf("%s\n", TIME_ERROR);
		return (-1);
	}
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

/* Current wall-clock time in microseconds, for precise sleeping. */
u_int64_t	get_time_us(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return ((tv.tv_sec * (u_int64_t)1000000) + tv.tv_usec);
}
