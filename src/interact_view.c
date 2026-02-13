/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_view.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 08:54:27 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 19:11:23 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	reset_view(t_fdf *d)
{
	d->state.angle_x = DEFAULT_ANGLE_X;
	d->state.angle_y = DEFAULT_ANGLE_Y;
	d->state.angle_z = DEFAULT_ANGLE_Z;
	d->state.x_offset = WIN_X / 2;
	d->state.y_offset = WIN_Y / 2;
	d->state.height_mod = DEFAULT_HEIGHT_MOD;
	init_zoom(d);
	d->time.img_need_redraw = true;
}

void	ortho_view_1(t_fdf *d)
{
	reset_view(d);
	d->state.angle_x = 0;
	d->state.angle_y = 0;
	d->state.angle_z = 0;
}

void	ortho_view_2(t_fdf *d)
{
	reset_view(d);
	d->state.angle_x = 1.5708;
	d->state.angle_y = 0;
	d->state.angle_z = 0;
}

void	ortho_view_3(t_fdf *d)
{
	reset_view(d);
	d->state.angle_x = 1.5708;
	d->state.angle_y = 1.5708;
	d->state.angle_z = 0;
}
