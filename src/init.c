/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 11:41:40 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_mlx_null(t_mlx *mlx)
{
	mlx->ptr = NULL;
	mlx->win = NULL;
	mlx->img.ptr = NULL;
}

static void	init_file(t_file *file, char *filename)
{
	file->name = filename;
	file->contents = NULL;
}

static void	init_map(t_map *map)
{
	map->data = NULL;
	map->index = NULL;
	map->widths = NULL;
}

static void	init_state(t_state *state)
{
	state->x_offset = DEFAULT_X_OFFSET;
	state->y_offset = DEFAULT_Y_OFFSET;
	state->height_mod = DEFAULT_HEIGHT_MOD;
	state->angle_x = DEFAULT_ANGLE_X;
	state->angle_y = DEFAULT_ANGLE_Y;
	state->angle_z = DEFAULT_ANGLE_Z;
	state->zoom = DEFAULT_ZOOM;
}

// Values to init
//	- mlx addresses to NULL for now
//	- file
//		- name + future chained list
//		- do not init fd, useless (we get it through open, first failure point)
//	- map
//		- all 3 array addresses
//	- starting states
//	- key/mouse values
//		- do not init repeat ratio, it is set to 0 automatically at repeat start
//		- do not init mouse pos, this is set on false->true lmb/rmb held change
//	- time values
//		- do not init last_key_repeat, it is set on key state ON->REPEAT change
//		- do not init current, it is set on mlx loop
void	init_prog(t_fdf *d, char *filename)
{
	init_mlx_null(&d->mlx);
	init_file(&d->file, filename);
	init_map(&d->map);
	init_state(&d->state);
	init_key(&d->key);
	init_mouse(&d->mouse);
	if (!init_time(&d->time))
		error_out(d, ERRTIME);
}
