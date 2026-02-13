/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_translate_mouse.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:17:29 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 09:28:23 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	move_x_mouse(t_fdf *d, int x)
{
	d->state.x_offset -= x;
	d->time.img_need_redraw = true;
}

void	move_y_mouse(t_fdf *d, int y)
{
	d->state.y_offset -= y;
	d->time.img_need_redraw = true;
}
