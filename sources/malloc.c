/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 19:18:40 by kcosta            #+#    #+#             */
/*   Updated: 2018/06/12 10:55:11 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;
t_malloc_zones	g_malloc_zones = { NULL, NULL, NULL };

static void	malloc_initialize(size_t size)
{
	void	*tiny;
	void	*small;

	if (!g_malloc_zones.tiny && IS_TINY(size))
	{
		tiny = mmap(NULL, TINY + ZONE_SIZE, PROT_READ | PROT_WRITE,
					MAP_ANON | MAP_PRIVATE, -1, 0);
		g_malloc_zones.tiny = (t_zone *)tiny;
		g_malloc_zones.tiny->zone = tiny + ZONE_SIZE;
		g_malloc_zones.tiny->cursor = ZONE_SIZE;
		g_malloc_zones.tiny->next = NULL;
		g_malloc_zones.tiny->metadata = NULL;
	}
	if (!g_malloc_zones.small && IS_SMALL(size))
	{
		small = mmap(NULL, SMALL + ZONE_SIZE, PROT_READ | PROT_WRITE,
					MAP_ANON | MAP_PRIVATE, -1, 0);
		g_malloc_zones.small = (t_zone *)small;
		g_malloc_zones.small->zone = small + ZONE_SIZE;
		g_malloc_zones.small->cursor = ZONE_SIZE;
		g_malloc_zones.small->next = NULL;
		g_malloc_zones.small->metadata = NULL;
	}
}

static void	*large_malloc(size_t size)
{
	void		*ret;
	t_metadata	*ptr;
	t_metadata	*new;

	ret = mmap(NULL, size, PROT_READ | PROT_WRITE,
				MAP_ANON | MAP_PRIVATE, -1, 0);
	new = (t_metadata *)ret;
	new->next = NULL;
	new->size = size;
	new->free = 0;
	new->data = ret + METADATA_SIZE;
	ptr = g_malloc_zones.large;
	if (!ptr)
		g_malloc_zones.large = new;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	ret += METADATA_SIZE;
	return (ret);
}

void		*ft_malloc(size_t size)
{
	void			*retval;

	size = ALIGN(size);
	malloc_initialize(size);
	if (size <= TINY_LIMIT)
		retval = tiny_malloc(size);
	else if (size <= SMALL_LIMIT)
		retval = small_malloc(size);
	else
		retval = large_malloc(size);
	return (retval);
}

void		*malloc(size_t size)
{
	void			*retval;

	pthread_mutex_lock(&g_mutex);
	size = ALIGN(size);
	malloc_initialize(size);
	if (size <= TINY_LIMIT)
		retval = tiny_malloc(size);
	else if (size <= SMALL_LIMIT)
		retval = small_malloc(size);
	else
		retval = large_malloc(size);
	pthread_mutex_unlock(&g_mutex);
	return (retval);
}
