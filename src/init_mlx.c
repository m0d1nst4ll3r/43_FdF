/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 00:59:19 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/09 19:14:08 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_mlx(t_fdf *d)
{
	d->mlx.ptr = mlx_init();
	if (!d->mlx.ptr)
		error_out(d, ERRMLX);
	d->mlx.win = mlx_new_window(d->mlx.ptr, WIN_X, WIN_Y, WIN_NAME);
	if (!d->mlx.win)
		error_out(d, ERRWIN);
	d->mlx.img.ptr = mlx_new_image(d->mlx.ptr, WIN_X, WIN_Y);
	if (!d->mlx.img.ptr)
		error_out(d, ERRIMG);
	d->mlx.img.addr = mlx_get_data_addr(d->mlx.img.ptr,
			&d->mlx.img.bpp, &d->mlx.img.llen, &d->mlx.img.endian);
}
