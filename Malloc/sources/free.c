/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 21:19:47 by kcosta            #+#    #+#             */
/*   Updated: 2018/06/11 10:04:51 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static int	malloc_metadata_free(t_metadata *ptr, void *addr)
{
	t_metadata	*prev;

	prev = NULL;
	while (ptr)
	{
		if (ptr->data == addr)
		{
			if (IS_TINY(ptr->size) || IS_SMALL(ptr->size))
				ptr->free = 1;
			else
			{
				if (prev)
					prev->next = ptr->next;
				else
					g_malloc_zones.large = ptr->next;
				munmap(ptr->data - METADATA_SIZE, ptr->size);
			}
			return (0);
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return (1);
}

static int	malloc_zone_free(t_zone *zone, void *ptr)
{
	int	retval;

	retval = 1;
	while (zone)
	{
		retval = malloc_metadata_free(zone->metadata, ptr);
		if (!retval)
			break ;
		zone = zone->next;
	}
	return (retval);
}

void		ft_free(void *ptr)
{
	t_zone	*zone;
	int		retval;

	zone = g_malloc_zones.tiny;
	retval = malloc_zone_free(zone, ptr);
	if (retval)
	{
		zone = g_malloc_zones.small;
		retval = malloc_zone_free(zone, ptr);
	}
	if (retval)
	{
		zone = g_malloc_zones.large;
		retval = malloc_metadata_free(zone, ptr);
	}
}

void		free(void *ptr)
{
	t_zone	*zone;
	int		retval;

	pthread_mutex_lock(&g_mutex);
	zone = g_malloc_zones.tiny;
	retval = malloc_zone_free(zone, ptr);
	if (retval)
	{
		zone = g_malloc_zones.small;
		retval = malloc_zone_free(zone, ptr);
	}
	if (retval)
	{
		zone = g_malloc_zones.large;
		retval = malloc_metadata_free(zone, ptr);
	}
	pthread_mutex_unlock(&g_mutex);
}
