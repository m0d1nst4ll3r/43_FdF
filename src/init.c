/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/04 15:50:31 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_mlx(t_fdf *d)
{
	d->mlx = mlx_init();
	if (!d->mlx)
		error_out(d, ERRMLX);
	d->win = mlx_new_window(d->mlx, WIN_X, WIN_Y, WIN_NAME);
	if (!d->win)
		error_out(d, ERRWIN);
	d->img.ptr = mlx_new_image(d->mlx, WIN_X, WIN_Y);
	if (!d->img.ptr)
		error_out(d, ERRIMG);
	d->img.addr = mlx_get_data_addr(d->img.ptr, &d->img.bpp, &d->img.llen,
			&d->img.endian);
}

void	init_keys(t_fdf *d)
{
	int	i;

	i = 0;
	while (i < NUM_KEYS)
	{
		d->key_state[i] = OFF;
		i++;
	}
}

void	init_prog(t_fdf *d, char **av)
{
	d->av = av;
	d->file = NULL;
	d->map = NULL;
	d->map_dat = NULL;
	d->map_widths = NULL;
	d->x_offset = X_OFFSET;
	d->y_offset = Y_OFFSET;
	d->point_distance = POINT_DISTANCE;
	d->height_mod = HEIGHT_MOD;
	d->line_offset = LINE_OFFSET;
	d->angle = START_ANGLE;
	d->redraw_needed = 1;
	init_keys(d);
	d->lmb_held = false;
	d->rmb_held = false;
	d->refresh_needed = false; // Everything in here is for fun, might remove
	d->frame_count = 0;
	gettimeofday(&d->old_time, NULL);
	d->fps_time = d->old_time; // End
	d->win = NULL;
	d->img.ptr = NULL;
	d->mlx = NULL;
}
