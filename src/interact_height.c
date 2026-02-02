/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_height.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:02:19 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 17:03:01 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	shift_down(t_fdf *d, int actions)
{
	d->height_mod -= actions;
	d->redraw_needed = true;
}

void	shift_up(t_fdf *d, int actions)
{
	d->height_mod += actions;
	d->redraw_needed = true;
}

