/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_zoom.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 12:15:33 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 19:04:28 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#define CTOP	0
#define CBOTTOM	1
#define CLEFT	2
#define CRIGHT	3

// Calculate zoom_x value so that wireframe model takes up 80% of WIN_X.
// Do the same for y. The smallest zoom value wins.
void	init_zoom(t_fdf *d)
{
	int		wireframe_width;
	int		wireframe_height;
	float	zoom_x;
	t_point	corners[4];

	if (!d->map.height)
		return ;
	set_point_init(d, d->map.widths[0] - 1, 0, &corners[CTOP]);
	set_point_init(d, 0, d->map.height - 1, &corners[CBOTTOM]);
	set_point_init(d, 0, 0, &corners[CLEFT]);
	set_point_init(d, d->map.widths[0] - 1, d->map.height - 1,
		&corners[CRIGHT]);
	wireframe_width = ft_abs(corners[CRIGHT].x - corners[CLEFT].x);
	wireframe_height = ft_abs(corners[CBOTTOM].y - corners[CTOP].y);
	zoom_x = ((float)WIN_X * 0.8) / wireframe_width;
	d->state.zoom = ((float)WIN_Y * 0.8) / wireframe_height;
	if (zoom_x < d->state.zoom)
		d->state.zoom = zoom_x;
}
