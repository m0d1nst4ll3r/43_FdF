# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rpohlen <rpohlen@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/20 14:37:51 by rpohlen           #+#    #+#              #
#    Updated: 2026/01/14 15:16:04 by rapohlen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES				= main.c

SRCDIR				= src
BUILDDIR			= .build

SRC					= $(addprefix $(SRCDIR)/, $(CFILES))
OBJ					= $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEP					= $(OBJ:.o=.d)

NAME				= fdf

INC					= inc \
					  libft/inc \
					  mlx
LIB					= libft/libft.a \
					  mlx/libmlx_Linux.a
LINK				= -lXext -lX11

CC					= cc
CFLAGS				= -Wall -Wextra -Werror -g
CPPFLAGS			= $(addprefix -I,$(INC)) -MMD -MP
MAKEFLAGS			+= --no-print-directory -j

all:				$(NAME)

bonus:				$(NAME)

$(NAME):			$(OBJ) $(LIB)
					$(CC) $(CFLAGS) $(CPPFLAGS) $(LINK) -o $@ $^

$(LIB):
					$(MAKE) -C $(@D)

$(BUILDDIR)/%.o:	$(SRCDIR)/%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
					$(MAKE) -C libft clean
					rm -rf $(BUILDDIR)

fclean:
					$(MAKE) -C libft fclean
					$(MAKE) -C mlx clean
					rm -rf $(NAME) $(BUILDDIR)

re:
					$(MAKE) fclean
					$(MAKE) all

-include $(DEP)

.PHONY:		all clean fclean re
