/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_translate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:53:42 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 16:54:40 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	move_down(t_fdf *d, int actions)
{
	d->y_offset += MOVE_SPEED * actions;
	d->redraw_needed = true;
}

void	move_up(t_fdf *d, int actions)
{
	d->y_offset -= MOVE_SPEED * actions;
	d->redraw_needed = true;
}

void	move_left(t_fdf *d, int actions)
{
	d->x_offset -= MOVE_SPEED * actions;
	d->redraw_needed = true;
}

void	move_right(t_fdf *d, int actions)
{
	d->x_offset += MOVE_SPEED * actions;
	d->redraw_needed = true;
}
