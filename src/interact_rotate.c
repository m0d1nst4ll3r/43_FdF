/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact_rotate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:01:29 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 17:01:44 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	shift_left(t_fdf *d, int actions)
{
	d->line_offset -= actions;
	d->redraw_needed = true;
}

void	shift_right(t_fdf *d, int actions)
{
	d->line_offset += actions;
	d->redraw_needed = true;
}
