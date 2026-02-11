/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_rotate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:01:29 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/11 20:04:34 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	rotate_x_increase(t_fdf *d, int actions)
{
	d->state.angle_x += ANGLE_MOVE * actions;
	d->time.img_need_redraw = true;
}

void	rotate_x_decrease(t_fdf *d, int actions)
{
	d->state.angle_x -= ANGLE_MOVE * actions;
	d->time.img_need_redraw = true;
}

void	rotate_y_increase(t_fdf *d, int actions)
{
	d->state.angle_y += ANGLE_MOVE * actions;
	d->time.img_need_redraw = true;
}

void	rotate_y_decrease(t_fdf *d, int actions)
{
	d->state.angle_y -= ANGLE_MOVE * actions;
	d->time.img_need_redraw = true;
}

void	rotate_z_increase(t_fdf *d, int actions)
{
	d->state.angle_z += ANGLE_MOVE * actions;
	d->time.img_need_redraw = true;
}

void	rotate_z_decrease(t_fdf *d, int actions)
{
	d->state.angle_z -= ANGLE_MOVE * actions;
	d->time.img_need_redraw = true;
}
