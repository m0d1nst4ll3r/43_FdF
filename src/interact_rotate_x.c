/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_rotate_x.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:19:48 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 09:19:59 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	rotate_x_increase(t_fdf *d)
{
	d->state.angle_x += ANGLE_MOVE * d->key.num_presses;
	d->time.img_need_redraw = true;
}

void	rotate_x_decrease(t_fdf *d)
{
	d->state.angle_x -= ANGLE_MOVE * d->key.num_presses;
	d->time.img_need_redraw = true;
}

void	rotate_x_mouse(t_fdf *d, int x)
{
	d->state.angle_x += ANGLE_MOVE / 7 * x;
	d->time.img_need_redraw = true;
}
