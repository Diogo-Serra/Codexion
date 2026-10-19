/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diserra <diosoare@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 22:00:56 by diserra           #+#    #+#             */
/*   Updated: 2026/10/12 22:23:21 by diserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Min-heap comparison logic
static int	req_less(t_request *a, t_request *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	if (a->id % 2 != b->id % 2)
		return (a->id % 2);
	return (a->id < b->id);
}

// Swap two heap requests
static void	req_swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

// Push a request onto the dongle's priority heap
void	heap_push(t_dongle *dongle, u_int64_t key, int id)
{
	t_request	*h;
	int			i;

	h = dongle->heap;
	i = dongle->size++;
	h[i].key = key;
	h[i].id = id;
	while (i > 0 && req_less(&h[i], &h[(i - 1) / 2]))
	{
		req_swap(&h[i], &h[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

// Pop the highest priority request from the heap
void	heap_pop(t_dongle *dongle)
{
	t_request	*h;
	int			i;
	int			child;

	h = dongle->heap;
	h[0] = h[--dongle->size];
	i = 0;
	while (2 * i + 1 < dongle->size)
	{
		child = 2 * i + 1;
		if (child + 1 < dongle->size && req_less(&h[child + 1], &h[child]))
			child++;
		if (!req_less(&h[child], &h[i]))
			break ;
		req_swap(&h[child], &h[i]);
		i = child;
	}
}

// Check if a coder is the next eligible user of a dongle
int	is_ready(t_coder *coder, t_dongle *dongle)
{
	return (dongle->size > 0 && dongle->heap[0].id == coder->id
		&& !dongle->taken && get_time() >= dongle->available_at);
}