/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:05:55 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/07 19:09:07 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Quick Bresenham explanation:
// 1. To draw a line between 2 points, first we consider the distance between
//		the two points on the x and y axes. That is the delta.
//	 When drawing the line, we will jump pixel by pixel. On one axis, we will
//		always jump a pixel, on the other, we will sometimes jump a pixel.
//	 One axis is "dominant" in a way.
//	 What determines which is the dominant axis is the x/y delta.
//		- Horizontal distance is longer:	abs(p2.x - p1.x) > abs(p2.y - p1.y)
//			= always jump a pixel on the X axis
//		-  Vertical  distance is longer:	abs(p2.x - p1.x) > abs(p2.y - p1.y)
//			= always jump a pixel on the Y axis
//	 We have one function for each case, where x and y have reversed roles.
// 2. If X is the dominant axis, and (p2.x < p1.x), we want to start from p2
//	 If Y is the dominant axis, and (p2.y < p1.y), we want to start from p2
//	 This is so we can just increment x or y and not bother about direction.
//	 We can simply call our function with (p1, p2) OR (p2, p1) to decide where
//		to start.
// 3. Finally, AFTER potentially swapping p2 and p1:
//	 If X is the dominant axis and (p2.y <  p1.y), we will be decrementing y
//	 If X is the dominant axis and (p2.y >= p1.y), we will be incrementing y
//	 If Y is the dominant axis and (p2.x <  p1.x), we will be decrementing x
//	 If Y is the dominant axis and (p2.x >= p1.x), we will be incrementing x
//	 This is the "direction" x or y will go
// 4a. There are 8 (2 * 2 * 2) different configurations.
//	 A way to visualize this is considering a circle with p1 at its center and
//		p2 somewhere around.
//	 Divide the circle in octants (eighths, or half quarters), and number
//		octants clockwise starting from 1
//	 Octants 2, 3, 6, 7 = horizontal distance is longer
//	 Octants 1, 4, 5, 8 =  vertical  distance is longer
// 4b. We can simplify these octants further by swapping p1 and p2 (point 2.)
//	 In octants 1, 8, consider p2 as the center of the circle, this translates
//		to octants 4, 5
//	 In octants 6, 7, consider p2 as the center of the circle, this translates
//		to octants 2, 3
// 4c. Finally, for point 3.:
//		Octant 2 - y decrements
//		Octant 3 - y increments
//		Octant 4 - x increments
//		Octant 5 - x decrements
// 5. Ok so we always increment x or y, and then the other sometimes increments
//		(or decrements). But how do we choose when to do it?
//	 Consider octant 3 (x dominant, no swap, y increases).
//	 To know where the real line is, 
//
static void	draw_line_x_dominant(t_img *img, t_point p1, t_point p2)
{
	t_point	delta;
	int		y_direction;
	int		error_term;

	delta.x = p2.x - p1.x;
	delta.y = p2.y - p1.y;
	y_direction = 1;
	if (delta.y < 0)
	{
		y_direction = -1;
		delta.y = -delta.y;
	}
	error_term = (2 * delta.y) - delta.x;
	while (p1.x < p2.x)
	{
		pixel_put(img, p1, DEFAULT_COLOR);
		if (error_term > 0)
		{
			p1.y += y_direction;
			error_term += 2 * (delta.y - delta.x);
		}
		else
			error_term += 2 * delta.y;
		p1.x++;
	}
}

static void	draw_line_y_dominant(t_img *img, t_point p1, t_point p2)
{
	t_point	delta;
	int		x_direction;
	int		error_term;

	delta.x = p2.x - p1.x;
	delta.y = p2.y - p1.y;
	x_direction = 1;
	if (delta.x < 0)
	{
		x_direction = -1;
		delta.x = -delta.x;
	}
	error_term = (2 * delta.x) - delta.y;
	while (p1.y < p2.y)
	{
		pixel_put(img, p1, DEFAULT_COLOR);
		if (error_term > 0)
		{
			p1.x += x_direction;
			error_term += 2 * (delta.x - delta.y);
		}
		else
			error_term += 2 * delta.x;
		p1.y++;
	}
}

// Draw a line between point p1 and point p2
static void	draw_line(t_img *img, t_point p1, t_point p2)
{
	if (ft_abs(p2.y - p1.y) < ft_abs(p2.x - p1.x))
	{
        if (p1.x > p2.x)
            draw_line_x_dominant(img, p2, p1);
        else
            draw_line_x_dominant(img, p1, p2);
	}
    else
	{
        if (p1.y > p2.y)
            draw_line_y_dominant(img, p2, p1);
        else
            draw_line_y_dominant(img, p1, p2);
	}
}

static t_point	transform(t_point point, int height, float angle)
{
	int	tmp;

	tmp = point.x;
	point.x = (tmp - point.y) * cos(angle);
	point.y = (tmp + point.y) * sin(angle) - height;
	return (point);
}

// Modified version for 3d transform
static void	link_points(t_fdf *d, t_point point)
{
	t_point	cur;
	t_point	right;
	t_point	below;

	cur.x = point.y + d->state.x_offset + point.x * POINT_DISTANCE;
	cur.y = d->state.y_offset + point.y * POINT_DISTANCE;
	if (point.y + 1 < d->map.height && point.x < d->map.widths[point.y + 1]) // bottom exists
	{
		below.x = (point.y + 1) + d->state.x_offset + point.x * POINT_DISTANCE;
		below.y = d->state.y_offset + (point.y + 1) * POINT_DISTANCE;
		draw_line(&d->mlx.img,
			transform(cur, d->map.index[point.y][point.x].z * d->state.height_mod,
				d->state.angle),
			transform(below, d->map.index[point.y + 1][point.x].z * d->state.height_mod,
				d->state.angle));
	}
	if (point.x + 1 < d->map.widths[point.y])
	{
		right.x = point.y + d->state.x_offset + (point.x + 1) * POINT_DISTANCE;
		right.y = d->state.y_offset + point.y * POINT_DISTANCE;
		draw_line(&d->mlx.img,
			transform(cur, d->map.index[point.y][point.x].z * d->state.height_mod,
				d->state.angle),
			transform(right, d->map.index[point.y][point.x + 1].z * d->state.height_mod,
				d->state.angle));
	}
}

/*static void	link_points(t_fdf *d, int x, int y)
{
	t_coord	cur;
	t_coord	right;
	t_coord	below;

	cur.x = y * d->line_offset + d->x_offset + x * d->point_distance;
	cur.y = d->y_offset + y * d->point_distance - d->map[y][x].height * d->height_mod;
	if (y + 1 < d->map_height && x < d->map_widths[y + 1]) // bottom exists
	{
		below.x = (y + 1) * d->line_offset + d->x_offset + x * d->point_distance;
		below.y = d->y_offset + (y + 1) * d->point_distance - d->map[y + 1][x].height * d->height_mod;
		draw_line(d->img, cur, below); // note that for color gradient we will need 4 args (colors)
	}
	if (x + 1 < d->map_widths[y])
	{
		right.x = y * d->line_offset + d->x_offset + (x + 1) * d->point_distance;
		right.y = d->y_offset + y * d->point_distance - d->map[y][x + 1].height * d->height_mod;
		draw_line(d->img, cur, right);
	}
}*/

bool	draw_image(t_fdf *d)
{
	bool	img_changed;
	t_point	point;

	img_changed = true; // TODO: fix later
	point.y = 0;
	while (point.y < d->map.height)
	{
		point.x = 0;
		while (point.x < d->map.widths[point.y])
		{
			link_points(d, point); // TODO: color TODO: not doing useless calculations for stuff outside of screen
			point.x++;
		}
		point.y++;
	}
	return (img_changed);
}

bool	reset_image(t_img *img)
{
	t_point	point;
	bool	img_changed;

	img_changed = false;
	point.y = 0;
	while (point.y < WIN_Y)
	{
		point.x = 0;
		while (point.x < WIN_X)
		{
			if (pixel_put(img, point, BACKGROUND_COLOR))
				img_changed = true;
			point.x++;
		}
		point.y++;
	}
	return (img_changed);
}

