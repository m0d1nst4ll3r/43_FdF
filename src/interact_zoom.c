/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_zoom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:00:51 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 17:01:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	zoom_in(t_fdf *d, int actions)
{
	d->point_distance += actions;
	d->redraw_needed = true;
}

void	zoom_out(t_fdf *d, int actions)
{
	d->point_distance -= actions;
	d->redraw_needed = true;
}
