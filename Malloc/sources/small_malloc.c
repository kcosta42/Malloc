/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 19:54:19 by kcosta            #+#    #+#             */
/*   Updated: 2018/06/06 21:14:05 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		*small_search_cache(size_t size, t_metadata *ptr)
{
	size_t		min;
	void		*retval;
	t_metadata	*prev;

	retval = NULL;
	prev = NULL;
	min = SMALL;
	while (ptr)
	{
		if (ptr->free && (size <= ptr->size) && ((ptr->size - size) < min))
		{
			if (prev)
				prev->free = 1;
			prev = ptr;
			ptr->free = 0;
			retval = ptr->data;
			min = ptr->size - size;
		}
		ptr = ptr->next;
	}
	return (retval);
}

static void		*small_search_free_list(size_t size)
{
	t_zone		*zone_ptr;
	t_metadata	*metadata_ptr;
	void		*retval;

	zone_ptr = g_malloc_zones.small;
	while (zone_ptr)
	{
		metadata_ptr = zone_ptr->metadata;
		retval = small_search_cache(size, metadata_ptr);
		if (retval)
			break ;
		zone_ptr = zone_ptr->next;
	}
	return (retval);
}

static void		small_extend_zone(void)
{
	t_zone	*new;
	t_zone	*ptr;
	void	*new_zone;

	ptr = g_malloc_zones.small;
	while (ptr->next)
		ptr = ptr->next;
	new_zone = mmap(NULL, SMALL + ZONE_SIZE, PROT_READ | PROT_WRITE,
					MAP_ANON | MAP_PRIVATE, -1, 0);
	new = (t_zone *)new_zone;
	new->zone = new_zone + ZONE_SIZE;
	new->cursor = ZONE_SIZE;
	new->next = NULL;
	new->metadata = NULL;
	ptr->next = new;
}

static void		small_insert_malloc(void *ret, size_t size)
{
	t_metadata	*new;
	t_metadata	*tmp;
	t_zone		*ptr;

	new = (t_metadata *)ret;
	new->size = size;
	new->free = 0;
	new->data = ret + METADATA_SIZE;
	ptr = g_malloc_zones.small;
	while (ptr->next)
		ptr = ptr->next;
	if (!ptr->metadata)
		ptr->metadata = new;
	else
	{
		tmp = ptr->metadata;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void			*small_malloc(size_t size)
{
	void	*retval;
	t_zone	*ptr;

	ptr = g_malloc_zones.small;
	if ((retval = small_search_free_list(size)))
		return (retval);
	while (ptr)
	{
		if ((ptr->cursor + size) <= SMALL)
		{
			retval = &ptr->zone[ptr->cursor];
			small_insert_malloc(retval, size);
			retval += METADATA_SIZE;
			ptr->cursor += size;
			break ;
		}
		if (!ptr->next)
			small_extend_zone();
		ptr = ptr->next;
	}
	return (retval);
}
