/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_zoom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:00:51 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 17:03:19 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	zoom_in(t_fdf *d, int actions)
{
	d->state.zoom *= 1 + actions * ZOOM_SPEED;
	d->time.img_state = IMG_NEED_REDRAW;
}

void	zoom_out(t_fdf *d, int actions)
{
	d->state.zoom /= 1 + actions * ZOOM_SPEED;
	d->time.img_state = IMG_NEED_REDRAW;
}
