/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_translate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:53:42 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/10 20:46:13 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	move_up(t_fdf *d, int actions)
{
	d->state.y_offset += actions;
	d->time.img_need_redraw = true;
}

void	move_down(t_fdf *d, int actions)
{
	d->state.y_offset -= actions;
	d->time.img_need_redraw = true;
}

void	move_left(t_fdf *d, int actions)
{
	d->state.x_offset += actions;
	d->time.img_need_redraw = true;
}

void	move_right(t_fdf *d, int actions)
{
	d->state.x_offset -= actions;
	d->time.img_need_redraw = true;
}
