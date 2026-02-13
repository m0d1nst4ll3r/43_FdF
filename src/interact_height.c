/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_height.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:02:19 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 14:41:10 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

void	shift_down(t_fdf *d)
{
	if (d->state.height_mod <= HEIGHT_MIN
		&& d->state.height_mod >= -HEIGHT_MIN)
		d->state.height_mod -= d->key.num_presses * HEIGHT_MOVE * HEIGHT_MIN;
	else if (d->state.height_mod > 0)
		d->state.height_mod /= 1 + d->key.num_presses * HEIGHT_MOVE;
	else
		d->state.height_mod *= 1 + d->key.num_presses * HEIGHT_MOVE;
	d->time.img_need_redraw = true;
}

void	shift_up(t_fdf *d)
{
	if (d->state.height_mod <= HEIGHT_MIN
		&& d->state.height_mod >= -HEIGHT_MIN)
		d->state.height_mod += d->key.num_presses * HEIGHT_MOVE * HEIGHT_MIN;
	else if (d->state.height_mod > 0)
		d->state.height_mod *= 1 + d->key.num_presses * HEIGHT_MOVE;
	else
		d->state.height_mod /= 1 + d->key.num_presses * HEIGHT_MOVE;
	d->time.img_need_redraw = true;
	printf("%f\n", d->state.height_mod);
}
