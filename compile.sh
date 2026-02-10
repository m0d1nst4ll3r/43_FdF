#!/bin/zsh
cc -Wall -Wextra -Werror -I mlx/ -I libft/inc color_gradient_test.c mlx/libmlx_Linux.a libft/libft.a -lXext -lX11 -lm
