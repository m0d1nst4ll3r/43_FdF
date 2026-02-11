/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_zoom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:00:51 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/11 16:26:16 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	zoom_in(t_fdf *d, int actions)
{
	d->state.zoom *= 1 + actions * ZOOM_SPEED;
	d->time.img_need_redraw = true;
}

void	zoom_out(t_fdf *d, int actions)
{
	d->state.zoom /= 1 + actions * ZOOM_SPEED;
	d->time.img_need_redraw = true;
}

void	zoom_in_mouse(t_fdf *d, int x, int y)
{
	zoom_in(d, 1);
	(void)x;
	(void)y;
	//d->state.x_offset += ZOOM_SPEED * x;
	//d->state.y_offset += ZOOM_SPEED * y;
}

void	zoom_out_mouse(t_fdf *d, int x, int y)
{
	zoom_out(d, 1);
	(void)x;
	(void)y;
	//d->state.x_offset -= ZOOM_SPEED * x;
	//d->state.y_offset -= ZOOM_SPEED * y;
}
