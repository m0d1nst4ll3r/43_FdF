/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_height.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:02:19 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/10 20:46:44 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	shift_down(t_fdf *d, int actions)
{
	if (d->state.height_mod <= HEIGHT_MIN
		&& d->state.height_mod >= - HEIGHT_MIN)
		d->state.height_mod -= actions * HEIGHT_MOVE / 5;
	else if (d->state.height_mod > 0)
		d->state.height_mod /= 1 + actions * HEIGHT_MOVE;
	else
		d->state.height_mod *= 1 + actions * HEIGHT_MOVE;
	d->time.img_need_redraw = true;
}

void	shift_up(t_fdf *d, int actions)
{
	if (d->state.height_mod <= HEIGHT_MIN
		&& d->state.height_mod >= - HEIGHT_MIN)
		d->state.height_mod += actions * HEIGHT_MOVE / 5;
	else if (d->state.height_mod > 0)
		d->state.height_mod *= 1 + actions * HEIGHT_MOVE;
	else
		d->state.height_mod /= 1 + actions * HEIGHT_MOVE;
	d->time.img_need_redraw = true;
}

