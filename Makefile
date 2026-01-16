# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rpohlen <rpohlen@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/20 14:37:51 by rpohlen           #+#    #+#              #
#    Updated: 2026/01/15 22:04:03 by rapohlen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source files
CFILES				= main.c \
					  init.c \
					  exit.c \
					  hook.c \
					  mlx_util.c
SRCDIR				= src
SRC					= $(addprefix $(SRCDIR)/, $(CFILES))

# Include directories
INCDIR				= inc \
					  libft/inc \
					  mlx

# Build directory
BUILDDIR			= .build

# Object and dependency files
OBJ					= $(SRC:%.c=$(BUILDDIR)/%.o)
DEP					= $(OBJ:.o=.d)

# Output binary
NAME				= fdf

# Libraries
LIB					= libft/libft.a \
					  mlx/libmlx_Linux.a
LINK				= -lXext -lX11

# Compiler settings
CC					= cc
CFLAGS				= -Wall -Wextra -Werror -MMD -MP $(addprefix -I,$(INCDIR))

# Make settings
MAKEFLAGS			+= --no-print-directory -j

# Default rule
all:				$(NAME)

# Link
$(NAME):			$(OBJ) $(LIB)
					$(CC) $(CFLAGS) $^ $(LINK) -o $@

# Build libraries
$(LIB):
					$(MAKE) -C $(@D)

# Compile source -> object (auto-create directories)
$(BUILDDIR)/%.o:	%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) -c -o $@ $<

# Cleanup
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

# Include dependency files
-include $(DEP)

.PHONY:		all clean fclean re
