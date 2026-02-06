/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_subfuncs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 01:11:54 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 01:16:19 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// I hate the norm. These could be statics in init.c, but noOooOo
void	init_mlx_null(t_mlx *mlx)
{
	mlx->ptr = NULL;
	mlx->win = NULL;
	mlx->img.ptr = NULL;
}

void	init_file(t_file *file, char *filename)
{
	file->name = filename;
	file->contents = NULL;
}

void	init_map(t_map *map)
{
	map->data = NULL;
	map->index = NULL;
	map->widths = NULL;
}

void	init_state(t_state *state)
{// TODO: STATES WILL EVENTUALLY GET VALUES THROUGH MATH & LOGIC, TO CENTER DRAWING
	state->x_offset = DEFAULT_X_OFFSET;
	state->y_offset = DEFAULT_Y_OFFSET;
	state->height_mod = DEFAULT_HEIGHT_MOD;
	state->angle = DEFAULT_ANGLE;
	state->zoom = DEFAULT_ZOOM;
}
