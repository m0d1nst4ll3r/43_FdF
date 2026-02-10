/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_zoom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:00:51 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/10 20:45:54 by rapohlen         ###   ########.fr       */
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
