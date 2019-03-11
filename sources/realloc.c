/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 21:37:46 by kcosta            #+#    #+#             */
/*   Updated: 2018/06/11 09:52:58 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*ptrdst;

	ptrdst = (unsigned char*)dst;
	while (n--)
		*ptrdst++ = *(unsigned char*)src++;
	return (dst);
}

static size_t	realloc_check_resize(t_metadata *ptr, void *addr,
									size_t size, size_t *old_size)
{
	size_t	diff;

	while (ptr)
	{
		if (ptr->data == addr)
		{
			*old_size = ptr->size;
			if (ptr->next)
			{
				diff = (ptr->next->data) - (addr);
				if (size <= diff && (IS_TINY(ptr->size)
								|| IS_SMALL(ptr->size)))
				{
					ptr->size = size;
					return (0);
				}
			}
			return (-1);
		}
		ptr = ptr->next;
	}
	return (1);
}

static int		realloc_resize(t_zone *zone, void *ptr,
							size_t size, size_t *old_size)
{
	int		retval;

	retval = 1;
	while (zone)
	{
		retval = realloc_check_resize(zone->metadata, ptr, size, old_size);
		if (!retval)
			return (0);
		else if (retval < 0)
			return (-1);
		zone = zone->next;
	}
	return (1);
}

static void		*realloc_return(void *to_free, void *addr)
{
	ft_free(to_free);
	pthread_mutex_unlock(&g_mutex);
	return (addr);
}

void			*realloc(void *ptr, size_t size)
{
	size_t	old_size;
	void	*new_zone;
	int		retval;

	old_size = size;
	pthread_mutex_lock(&g_mutex);
	new_zone = NULL;
	if (!ptr || !size)
		return (realloc_return(ptr, ft_malloc(size)));
	retval = realloc_resize(g_malloc_zones.tiny, ptr, size, &old_size);
	if (1 == retval)
		retval = realloc_resize(g_malloc_zones.small, ptr, size, &old_size);
	if (1 == retval)
		retval =
			realloc_check_resize(g_malloc_zones.large, ptr, size, &old_size);
	if (retval)
	{
		new_zone = (1 != retval) ? ft_malloc(size) : NULL;
		(retval != 1) ? ft_memcpy(new_zone, ptr, old_size) : 0;
		return (realloc_return(ptr, new_zone));
	}
	else
		return (realloc_return(NULL, ptr));
}
