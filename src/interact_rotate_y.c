/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_rotate_y.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:20:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 10:05:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	rotate_y_increase(t_fdf *d)
{
	d->state.angle_y += ANGLE_MOVE * d->key.num_presses;
	d->time.img_need_redraw = true;
}

void	rotate_y_decrease(t_fdf *d)
{
	d->state.angle_y -= ANGLE_MOVE * d->key.num_presses;
	d->time.img_need_redraw = true;
}

void	rotate_y_mouse(t_fdf *d, int y)
{
	d->state.angle_y -= ANGLE_MOVE / 7 * y;
	d->time.img_need_redraw = true;
}
