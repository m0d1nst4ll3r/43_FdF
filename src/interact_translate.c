/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_translate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:53:42 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/03 12:24:19 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	move_y(t_fdf *d, int actions)
{
	d->y_offset += actions;
	d->redraw_needed = true;
}

void	move_x(t_fdf *d, int actions)
{
	d->x_offset += actions;
	d->redraw_needed = true;
}
