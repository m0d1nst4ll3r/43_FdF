/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:05:55 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 14:35:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	link_points(t_fdf *d, int x, int y)
{
	t_point	cur;
	t_point	right;
	t_point	below;

	set_point(d, x, y, &cur);
	if (x + 1 < d->map.widths[y])
	{
		set_point(d, x + 1, y, &right);
		if (!((cur.x < 0 && right.x < 0)
				|| (cur.x >= WIN_X && right.x >= WIN_X)
				|| (cur.y < 0 && right.y < 0)
				|| (cur.y >= WIN_Y && right.y >= WIN_Y)))
			draw_line(&d->mlx.img, cur, right);
	}
	if (y + 1 < d->map.height && x < d->map.widths[y + 1])
	{
		set_point(d, x, y + 1, &below);
		if (!((cur.x < 0 && below.x < 0)
				|| (cur.x >= WIN_X && below.x >= WIN_X)
				|| (cur.y < 0 && below.y < 0)
				|| (cur.y >= WIN_Y && below.y >= WIN_Y)))
			draw_line(&d->mlx.img, cur, below);
	}
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
			if (x + 1 < d->map.widths[y]
				|| (y + 1 < d->map.height && x < d->map.widths[y + 1]))
				link_points(d, x, y);
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
