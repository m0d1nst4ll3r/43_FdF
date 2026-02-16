/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_3d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:25:44 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/16 14:14:14 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	transform_rotate_x(t_point *point, float angle)
{
	int	tmp;

	tmp = point->y;
	point->y = tmp * cos(angle) - point->z * sin(angle);
	point->z = tmp * sin(angle) + point->z * cos(angle);
}

static void	transform_rotate_y(t_point *point, float angle)
{
	int	tmp;

	tmp = point->x;
	point->x = point->z * sin(angle) + tmp * cos(angle);
	point->z = point->z * cos(angle) - tmp * sin(angle);
}

static void	transform_rotate_z(t_point *point, float angle)
{
	int	tmp;

	tmp = point->x;
	point->x = tmp * cos(angle) - point->y * sin(angle);
	point->y = tmp * sin(angle) + point->y * cos(angle);
}

// Project a x, y, z point onto the screen's x, y
// Using 3 rotational transforms
// Points are normalized according to model center
// POINT_DISTANCE and zoom are used in conjunction to gain precision
void	set_point(t_fdf *d, int x, int y, t_point *point)
{
	point->x = POINT_DISTANCE * (x - (d->map.widths[0] - 1) / 2.0);
	point->y = POINT_DISTANCE * (y - (d->map.height - 1) / 2.0);
	point->z = POINT_DISTANCE * d->map.index[y][x].z * d->state.height_mod;
	point->color = d->map.index[y][x].color;
	transform_rotate_x(point, d->state.angle_x);
	transform_rotate_y(point, d->state.angle_y);
	transform_rotate_z(point, d->state.angle_z);
	point->x = point->x * d->state.zoom + d->state.x_offset;
	point->y = point->y * d->state.zoom + d->state.y_offset;
}

void	set_point_init(t_fdf *d, int x, int y, t_point *point)
{
	point->x = POINT_DISTANCE * x;
	point->y = POINT_DISTANCE * y;
	point->z = 0;
	transform_rotate_x(point, d->state.angle_x);
	transform_rotate_y(point, d->state.angle_y);
	transform_rotate_z(point, d->state.angle_z);
	point->x = point->x;
	point->y = point->y;
}
