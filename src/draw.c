/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:05:55 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 17:20:32 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	draw_line_low(t_img img, t_point p1, t_point p2)
{
	t_point	t;
	t_point	i;
	int		yi;
	int		d;

	t.x = p2.x - p1.x;
	t.y = p2.y - p1.y;
	yi = 1;
	if (t.y < 0)
	{
		yi = -1;
		t.y = -t.y;
	}
	d = (2 * t.y) - t.x;
	i.y = p1.y;
	i.x = p1.x;
	while (i.x < p2.x)
	{
		pixel_put(img, i.x, i.y, 0xffffff);
		if (d > 0)
		{
			i.y += yi;
			d += 2 * (t.y - t.x);
		}
		else
			d += 2 * t.y;
		i.x++;
	}
}

static void	draw_line_high(t_img img, t_point p1, t_point p2)
{
	t_point	t;
	t_point	i;
	int		xi;
	int		d;

	t.x = p2.x - p1.x;
	t.y = p2.y - p1.y;
	xi = 1;
	if (t.x < 0)
	{
		xi = -1;
		t.x = -t.x;
	}
	d = (2 * t.x) - t.y;
	i.y = p1.y;
	i.x = p1.x;
	while (i.y < p2.y)
	{
		pixel_put(img, i.x, i.y, 0xffffff);
		if (d > 0)
		{
			i.x += xi;
			d += 2 * (t.x - t.y);
		}
		else
			d += 2 * t.x;
		i.y++;
	}
}

// Draw a line between point p1 and point p2
static void	draw_line(t_img img, t_point p1, t_point p2)
{
	if (ft_abs(p2.y - p1.y) < ft_abs(p2.x - p1.x))
	{
        if (p1.x > p2.x)
            draw_line_low(img, p2, p1);
        else
            draw_line_low(img, p1, p2);
	}
    else
	{
        if (p1.y > p2.y)
            draw_line_high(img, p2, p1);
        else
            draw_line_high(img, p1, p2);
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
static void	link_points(t_fdf *d, int x, int y)
{
	t_point	cur;
	t_point	right;
	t_point	below;

	cur.x = y + d->x_offset + x * d->point_distance;
	cur.y = d->y_offset + y * d->point_distance;
	if (y + 1 < d->map_height && x < d->map_widths[y + 1]) // bottom exists
	{
		below.x = (y + 1) + d->x_offset + x * d->point_distance;
		below.y = d->y_offset + (y + 1) * d->point_distance;
		draw_line(d->img, transform(cur, d->map[y][x].z * d->height_mod, d->angle),
				transform(below, d->map[y + 1][x].z * d->height_mod, d->angle));
	}
	if (x + 1 < d->map_widths[y])
	{
		right.x = y + d->x_offset + (x + 1) * d->point_distance;
		right.y = d->y_offset + y * d->point_distance;
		draw_line(d->img, transform(cur, d->map[y][x].z * d->height_mod, d->angle),
				transform(right, d->map[y][x + 1].z * d->height_mod, d->angle));
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

void	draw_image(t_fdf *d)
{
	int	x;
	int	y;

	y = 0;
	while (y < d->map_height)
	{
		x = 0;
		while (x < d->map_widths[y])
		{
			link_points(d, x, y); // TODO: color TODO: not doing useless calculations for stuff outside of screen
			x++;
		}
		y++;
	}
	d->refresh_needed = 1;
}

bool	reset_image(t_img *img)
{
	t_point	point;
	bool	changed;

	changed = false;
	point.y = 0;
	while (point.y < WIN_Y)
	{
		point.x = 0;
		while (point.x < WIN_X)
		{
			if (pixel_put(img, point, 0))
				changed = true;
			point.x++;
		}
		point.y++;
	}
	return (changed);
}

