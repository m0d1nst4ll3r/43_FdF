/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_zoom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:00:51 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 09:30:07 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	zoom_in(t_fdf *d)
{
	d->state.zoom *= 1 + d->key.num_presses * ZOOM_SPEED;
	d->time.img_need_redraw = true;
}

void	zoom_out(t_fdf *d)
{
	d->state.zoom /= 1 + d->key.num_presses * ZOOM_SPEED;
	d->time.img_need_redraw = true;
}

void	zoom_in_mouse(t_fdf *d)
{
	d->state.zoom *= 1 + d->key.num_presses * ZOOM_SPEED * 3;
	d->time.img_need_redraw = true;
}

void	zoom_out_mouse(t_fdf *d)
{
	d->state.zoom /= 1 + d->key.num_presses * ZOOM_SPEED * 3;
	d->time.img_need_redraw = true;
}
