/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 21:52:58 by kcosta            #+#    #+#             */
/*   Updated: 2018/06/12 10:54:39 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void		print_unsigned(size_t addr, size_t base)
{
	char	value;

	if (!addr)
	{
		base == 16 ? write(1, "0x", 2) : 0;
		return ;
	}
	print_unsigned(addr / base, base);
	value = HEX_STRING[(addr % base)];
	write(1, &value, 1);
}

static void	print_malloc_metadata(t_metadata *ptr)
{
	void	*addr;

	addr = ptr->data - METADATA_SIZE;
	print_unsigned((size_t)addr + METADATA_SIZE, 16);
	write(1, " - ", 3);
	print_unsigned((size_t)(addr + ptr->size), 16);
	write(1, " : ", 3);
	print_unsigned(ptr->size - METADATA_SIZE, 10);
	write(1, " octets\n", 8);
}

static void	print_all_mallocs(t_zone *ptr)
{
	t_metadata	*tmp;

	while (ptr)
	{
		tmp = ptr->metadata;
		while (tmp)
		{
			if (!tmp->free)
				print_malloc_metadata(tmp);
			tmp = tmp->next;
		}
		ptr = ptr->next;
	}
}

static void	print_large_zone(void)
{
	t_metadata	*ptr;

	ptr = g_malloc_zones.large;
	while (ptr)
	{
		print_malloc_metadata(ptr);
		ptr = ptr->next;
	}
}

void		show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mutex);
	if (g_malloc_zones.tiny)
	{
		write(1, "TINY:  ", 7);
		print_unsigned((size_t)g_malloc_zones.tiny, 16);
		write(1, "\n", 1);
		print_all_mallocs(g_malloc_zones.tiny);
	}
	if (g_malloc_zones.small)
	{
		write(1, "SMALL: ", 7);
		print_unsigned((size_t)g_malloc_zones.small, 16);
		write(1, "\n", 1);
		print_all_mallocs(g_malloc_zones.small);
	}
	if (g_malloc_zones.large)
	{
		write(1, "LARGE: ", 7);
		print_unsigned((size_t)g_malloc_zones.large, 16);
		write(1, "\n", 1);
		print_large_zone();
	}
	pthread_mutex_unlock(&g_mutex);
}
