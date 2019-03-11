/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 19:19:46 by kcosta            #+#    #+#             */
/*   Updated: 2018/06/06 22:43:16 by kcosta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>

# include <sys/mman.h>
# include <sys/types.h>

# include <pthread.h>

# define HEX_STRING "0123456789ABCDEF"

# define METADATA_SIZE	sizeof(t_metadata)
# define ZONE_SIZE		sizeof(t_zone)

# define ALIGN_MOD(x) ((x % 16) || !x) ? (x + 16) - (x % 16) : x
# define ALIGN(x) ALIGN_MOD((x + METADATA_SIZE))

# define COEFF 			200
# define PAGE_SIZE		getpagesize()

# define TINY			(size_t)(PAGE_SIZE * 16 * 1024)
# define TINY_LIMIT		(TINY / COEFF)
# define IS_TINY(x)		(x <= TINY_LIMIT)

# define SMALL			(size_t)(PAGE_SIZE * 128 * 1024)
# define SMALL_LIMIT	(SMALL / COEFF)
# define IS_SMALL(x)	((x > TINY_LIMIT) && (x <= SMALL_LIMIT))

typedef struct			s_metadata
{
	struct s_metadata	*next;
	size_t				size;
	unsigned char		free;
	void				*data;
}						t_metadata;

typedef struct			s_zone
{
	struct s_zone		*next;
	t_metadata			*metadata;
	size_t				cursor;
	void				*zone;
}						t_zone;

typedef struct			s_malloc_zones
{
	t_zone				*tiny;
	t_zone				*small;
	t_metadata			*large;
}						t_malloc_zones;

extern t_malloc_zones	g_malloc_zones;
extern pthread_mutex_t	g_mutex;

void					*tiny_malloc(size_t size);
void					*small_malloc(size_t size);

void					*ft_malloc(size_t size);
void					ft_free(void *ptr);

void					show_alloc_mem(void);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					*calloc(size_t count, size_t size);
void					free(void *ptr);

#endif
