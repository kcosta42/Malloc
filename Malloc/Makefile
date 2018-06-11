# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/12 10:00:00 by kcosta            #+#    #+#              #
#    Updated: 2018/06/06 22:33:16 by kcosta           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# =========== Colors =========== #
CLEAR			:= "\033[2K"
UP				:= "\033[A"
EOC				:= "\033[0;0m"
BLACK			:= "\033[0;30m"
RED				:= "\033[0;31m"
GREEN			:= "\033[0;32m"
YELLOW			:= "\033[0;33m"
BLUE			:= "\033[0;34m"
PURPLE			:= "\033[0;35m"
CYAN			:= "\033[0;36m"
WHITE			:= "\033[0;37m"
# ============================== #

# ========== Editable ========== #
NAME			:= libft_malloc_$(HOSTTYPE).so
LIBNAME			:= libft_malloc.so
# ============================== #

# ========== Standard ========== #
CC				:= gcc
FLAGS			:= -Wall -Wextra -Werror
# ============================== #

# =========== Files ============ #
SRCS_FILES		:=	malloc.c			\
					free.c				\
					realloc.c			\
					calloc.c			\
					show_alloc_mem.c	\
					tiny_malloc.c		\
					small_malloc.c
HEADERS_FILES	:=	malloc.h
# ============================== #

# ========== Sources =========== #
SRCS_PATH		:= sources/
SRCS			:= $(addprefix $(SRCS_PATH), $(SRCS_FILES))
# ============================== #

# ========== Objects =========== #
OBJS_PATH		:= objs/
OBJS			:= $(addprefix $(OBJS_PATH), $(SRCS_FILES:.c=.o))
# ============================== #

# ========== Includes ========== #
INCLUDES_PATH	:= includes/
INCLUDES		:= -I $(INCLUDES_PATH)
HEADERS			:= $(addprefix $(INCLUDES_PATH), $(HEADERS_FILES))
# ============================== #

.PHONY: all clean fclean re

all: $(NAME)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c $(HEADERS)
	@mkdir $(OBJS_PATH) 2> /dev/null || true
	$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $<

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -shared $(OBJS) -o $(NAME)
	ln -fs $@ $(LIBNAME)

clean:
	@rm -fv $(OBJS)
	@rmdir $(OBJS_PATH) 2> /dev/null || true

fclean: clean
	@rm -fv $(NAME)
	@rm -fv $(LIBNAME)

re: fclean all
