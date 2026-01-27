/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/27 15:00:20 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_mlx(t_fdf *d)
{
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
			&d->img.endian);
}

void	init_prog(t_fdf *d, char **av)
{
	d->av = av;
	d->file = NULL;
	d->map = NULL;
	d->map_dat = NULL;
	d->map_widths = NULL;
	d->lmb_held = 0; // Begin
	d->rmb_held = 0;
	d->refresh_needed = false; // Everything in here is for fun, might remove
	d->frame_count = 0;
	gettimeofday(&d->old_time, NULL);
	d->fps_time = d->old_time; // End
	d->win = NULL;
	d->img.img = NULL;
	d->mlx = NULL;
}
