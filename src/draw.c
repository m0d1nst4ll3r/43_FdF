/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:05:55 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/11 15:58:08 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Quick Bresenham explanation:
// 1. To draw a line between 2 points, consider the distance between the two
//		points on the x and y axes. That is the delta.
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
// 5. What is error_term? How do we decide whether to jump a pixel or not?
//	 It's too complicated to explain here. It has to do with math, transforming
//		float arithmetic into integer arithmetic, and how to decide whether the
//		real geometric line drawn between two pixels (two points at the centers
//		of the pixels) has crossed a new pixel's threshold or not.
//	 For a very quick explanation, the real geometric line's yi coordinate is
//		calculated, at coordinate xi, as (for octant 3) xi * dy / dx.
//		yi = xi * dy / dx
//		The next pixel threshold is yi + 0.5 (half a pixel downwards).
//		Whether to jump a pixel or not is decided with:
//		xi * dy / dx > yi + 0.5 ?
//		Or, in english: is the real line beyond the next pixel's threshold?
//		Simplify by putting all variables on one side:
//		xi * dy / dx - yi - 0.5 > 0 ?
//		This has float arithmetic, we want integer. So, multiply by dx:
//		xi * dy - yi * dx - 0.5dx > 0 ?
//		And multiply by 2:
//		2 xi*dy - 2 yi*dx - dx > 0 ?
//		xi and yi are 0 when we start
//		error_term = -dx
//		Every time x increments, add 2 dy to error_term.
//		Decide whether to jump y or not (error_term > 0 ?).
//		If y increments, subtract 2 dx from error_term.
// 6. Colors
//	 First, we always decompose colors (RGB channels are not linear in an int).
//	 Then, we used fixed-point arithmetic to avoid integer truncation and drift
//		issues (dividing a hypothetical R value of 80 by 81 yields 0).
//	 This avoids floating point operations and produces acceptable precision.
//	 Then, we just do (c2 - c1) / (pixel_count - 1) to find color step, and add
//		it to the starting color every iteration.
static void	draw_line_x_dominant(t_img *img, t_point p1, t_point p2)
{
	t_bresenham	b;

	b.delta.x = p2.x - p1.x;
	b.delta.y = p2.y - p1.y;
	b.step = 1;
	if (b.delta.y < 0)
	{
		b.step = -1;
		b.delta.y = -b.delta.y;
	}
	b.error_term = -b.delta.x;
	b.c_i = decompose_color(p1.color);
	get_color_step(b.c_i, decompose_color(p2.color), b.delta.x, &b.c_step);
	while (p1.x <= p2.x)
	{
		if (b.error_term > 0)
		{
			p1.y += b.step;
			b.error_term -= 2 * b.delta.x;
		}
		pixel_put(img, p1, recompose_color(b.c_i));
		b.error_term += 2 * b.delta.y;
		p1.x++;
		add_colors(&b.c_i, b.c_step);
	}
}

// Draw from points[0] to points[1]
static void	draw_line_y_dominant(t_img *img, t_point p1, t_point p2)
{
	t_bresenham	b;

	b.delta.x = p2.x - p1.x;
	b.delta.y = p2.y - p1.y;
	b.step = 1;
	if (b.delta.x < 0)
	{
		b.step = -1;
		b.delta.x = -b.delta.x;
	}
	b.error_term = -b.delta.y;
	b.c_i = decompose_color(p1.color);
	get_color_step(b.c_i, decompose_color(p2.color), b.delta.y, &b.c_step);
	while (p1.y <= p2.y)
	{
		if (b.error_term > 0)
		{
			p1.x += b.step;
			b.error_term -= 2 * b.delta.y;
		}
		pixel_put(img, p1, recompose_color(b.c_i));
		b.error_term += 2 * b.delta.x;
		p1.y++;
		add_colors(&b.c_i, b.c_step);
	}
}

// Draw a line between point p1 and point p2
static void	draw_line(t_img *img, t_point p1, t_point p2)
{
	if (ft_abs(p2.x - p1.x) > ft_abs(p2.y - p1.y))
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

static void	transform_3d(t_point *point, int height, float angle)
{
	int	tmp;

	tmp = point->x;
	point->x = (tmp - point->y) * cos(angle);
	point->y = (tmp + point->y) * sin(angle) - height;
}

static void	link_right(t_fdf *d, int x, int y)
{
	t_point	p1;
	t_point	p2;

	p1.x = x * POINT_DISTANCE;
	p1.y = y * POINT_DISTANCE;
	p1.color = d->map.index[y][x].color;
	p2.x = p1.x + POINT_DISTANCE;
	p2.y = p1.y;
	p2.color = d->map.index[y][x + 1].color;
	transform_3d(&p1, d->map.index[y][x].z * d->state.height_mod, d->state.angle);
	transform_3d(&p2, d->map.index[y][x + 1].z * d->state.height_mod, d->state.angle);
	p1.x *= d->state.zoom;
	p1.y *= d->state.zoom;
	p2.x *= d->state.zoom;
	p2.y *= d->state.zoom;
	p1.x += d->state.x_offset;
	p1.y += d->state.y_offset;
	p2.x += d->state.x_offset;
	p2.y += d->state.y_offset;
	draw_line(&d->mlx.img, p1, p2);
}

static void	link_below(t_fdf *d, int x, int y)
{
	t_point	p1;
	t_point	p2;

	p1.x = x * POINT_DISTANCE;
	p1.y = y * POINT_DISTANCE;
	p1.color = d->map.index[y][x].color;
	p2.x = p1.x;
	p2.y = p1.y + POINT_DISTANCE;
	p2.color = d->map.index[y + 1][x].color;
	transform_3d(&p1, d->map.index[y][x].z * d->state.height_mod, d->state.angle);
	transform_3d(&p2, d->map.index[y + 1][x].z * d->state.height_mod, d->state.angle);
	p1.x *= d->state.zoom;
	p1.y *= d->state.zoom;
	p2.x *= d->state.zoom;
	p2.y *= d->state.zoom;
	p1.x += d->state.x_offset;
	p1.y += d->state.y_offset;
	p2.x += d->state.x_offset;
	p2.y += d->state.y_offset;
	draw_line(&d->mlx.img, p1, p2);
}

void	draw_image(t_fdf *d)
{
	int	x;
	int	y;

	y = 0;
	while (y < d->map.height)
	{
		x = 0;
		while (x < d->map.widths[y])
		{
			if (x + 1 < d->map.widths[y])
				link_right(d, x, y);
			if (y + 1 < d->map.height && x < d->map.widths[y + 1])
				link_below(d, x, y);
			// TODO: not doing useless calculations for stuff outside of screen
			x++;
		}
		y++;
	}
}

void	reset_image(t_img *img)
{
	t_point	point;

	point.y = 0;
	while (point.y < WIN_Y)
	{
		point.x = 0;
		while (point.x < WIN_X)
		{
			pixel_put(img, point, BACKGROUND_COLOR);
			point.x++;
		}
		point.y++;
	}
}

