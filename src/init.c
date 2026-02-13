/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 16:54:08 by rapohlen         ###   ########.fr       */
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

// We are not setting zoom, this will be done after building map
// We need map values to know what size the model should be to fit on screen
static void	init_state(t_state *state)
{
	state->angle_x = DEFAULT_ANGLE_X;
	state->angle_y = DEFAULT_ANGLE_Y;
	state->angle_z = DEFAULT_ANGLE_Z;
	state->height_mod = DEFAULT_HEIGHT_MOD;
	state->x_offset = WIN_X / 2;
	state->y_offset = WIN_Y / 2;
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
