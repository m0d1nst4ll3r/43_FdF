/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_win_pos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 12:15:33 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 16:27:37 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#define CTOP	0
#define CBOTTOM	1
#define CLEFT	2
#define CRIGHT	3

// Calculate zoom_x value so that wireframe model takes up 80% of WIN_X.
// Do the same for y. The smallest zoom value wins.
void	init_win_pos(t_fdf *d)
{
	int		wireframe_width;
	int		wireframe_height;
	float	zoom_x;
	t_point	corners[4];

	set_point_init(d, d->map.widths[0] - 1, 0, &corners[CTOP]);
	set_point_init(d, 0, d->map.height - 1, &corners[CBOTTOM]);
	set_point_init(d, 0, 0, &corners[CLEFT]);
	set_point_init(d, d->map.widths[0] - 1, d->map.height - 1, &corners[CRIGHT]);
	wireframe_width = ft_abs(corners[CRIGHT].x - corners[CLEFT].x);
	wireframe_height = ft_abs(corners[CBOTTOM].y - corners[CTOP].y);
	ft_printf("H%d W%d\n", wireframe_height, wireframe_width);
	zoom_x = ((float)WIN_X * 0.8) / wireframe_width;
	d->state.zoom = ((float)WIN_Y * 0.8) / wireframe_height;
	if (zoom_x < d->state.zoom)
		d->state.zoom = zoom_x;
	d->state.x_offset = 0;//(WIN_X - wireframe_width * d->state.zoom) / 2;
	d->state.y_offset = 0;//(WIN_Y - wireframe_height * d->state.zoom) / 2
		//+ corners[CLEFT].y - corners[CTOP].y;
}
/*

x = widths[0] * cos(y) * cos(z) - height * sin(z)

y = y * cos(x)
x = x * cos(y)
y = x * cos(y) * sin(z) + y * cos(x) * cos(z)

b - a
b: x 0, y height
a: x widths[0], y 0
Yb = height * cos(x) * cos(z)
Ya = widths[0] * cos(y) * sin(z)

*/
