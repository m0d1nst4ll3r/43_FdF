#!/bin/zsh
cc -Wall -Wextra -Werror -I mlx/ color_gradient_test.c mlx/libmlx_Linux.a -lXext -lX11 -lm
