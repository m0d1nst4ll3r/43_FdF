/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/19 11:48:00 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_mlx(t_fdf *d)
{
	d->win = NULL;
	d->img.img = NULL;
	d->mlx = mlx_init();
	if (!d->mlx)
		error_out(*d, ERRMLX);
	d->win = mlx_new_window(d->mlx, WIN_X, WIN_Y, WIN_NAME);
	if (!d->win)
		error_out(*d, ERRWIN);
	d->img.img = mlx_new_image(d->mlx, WIN_X, WIN_Y);
	if (!d->img.img)
		error_out(*d, ERRIMG);
	d->img.addr = mlx_get_data_addr(d->img.img, &d->img.bpp, &d->img.llen,
			&d->img.endian); //cannot fail
}

void	init_prog(t_fdf *d, char **av)
{
	d->av = av; // copy av
	d->file = NULL; // file list
	d->map = NULL; // map array
	d->lmb_held = 0;
	d->rmb_held = 0;
	d->refresh_needed = false;
	d->frame_count = 0;
	gettimeofday(&d->old_time, NULL);
	d->fps_time = d->old_time;
	init_mlx(d); // init mlx
	// init mallocs (none for now)
}
