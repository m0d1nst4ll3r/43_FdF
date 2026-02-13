/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_rotate_z.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:01:29 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 09:20:52 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	rotate_z_increase(t_fdf *d)
{
	d->state.angle_z += ANGLE_MOVE * d->key.num_presses;
	d->time.img_need_redraw = true;
}

void	rotate_z_decrease(t_fdf *d)
{
	d->state.angle_z -= ANGLE_MOVE * d->key.num_presses;
	d->time.img_need_redraw = true;
}

void	rotate_z_mouse(t_fdf *d, int z)
{
	d->state.angle_z += ANGLE_MOVE / 7 * z;
	d->time.img_need_redraw = true;
}
