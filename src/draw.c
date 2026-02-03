/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:05:55 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/03 13:56:17 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	draw_line_low(t_img img, t_coord p1, t_coord p2)
{
	t_coord	t;
	t_coord	i;
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

static void	draw_line_high(t_img img, t_coord p1, t_coord p2)
{
	t_coord	t;
	t_coord	i;
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
static void	draw_line(t_img img, t_coord p1, t_coord p2)
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

// This is it
// Now
// We need to link every point together
// We're starting top left
// Finishing bottom right
// Logically we only need to link each point to its right & down neighbours (if they exist)
// So we're checking for validity of x+1 and y+1 then choosing them as links for bresenham bullshit
// Note that FOR NOW points should always be to the right (on the same line) or downwards (left, same col, or right)
// But as we move on points will maybe be in any configuration (all 8 octants in bresenham)
// Summarizing:
// 1. 2 functions calls (right, down) MAX, 0 minimum (no neighbours)
// 2. Points can be in any octant configuration
static void	link_points(t_fdf *d, int x, int y)
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
}

// We're gonna try to place points on the screen, according to where they should be on the map
// Ex
// 0 0 0
// 0 0 0
// 0 0 0
// We should have 9 points in a rectangular grid (for now, no angle)
// We need to figure out the distance between points
// We'll display points as little red circles for now (reusing the mlx_paint circle draw algorithm)
// Distance between points for now will just be a macro that I can define
// We'll figure out how to center the thing later
// Step 1 achieved: we can draw circles where points should be (without height)
// Next step, height
// Step 2 achieved: height works
// This is ugly cause there's no isometric effect, next step: isometric
// Now we're getting into the interesting stuff
// A basic isometric effect is just that each new line should have a X offset that grows every line
// Let's try that
// We're getting something interesting here, but let's add keys to modify them in real time and see how the image changes
// This way we can get an idea of what looks right and what doesn't, what looks like rotation, etc...
// We need keys for everything:
// X/Y offsets - that's arrow keys cause why not
// Point distance - this should have some kind of zoom effect, we'll place it on shift/ctrl
// Line offset - AD for now
// XZ for now will move circle_radius (clamped between 1 and something...)
// FR will move height_mod
// Step 3 achieved: all hooks are set and I can move my shit around
// It's ugly, but that's ok. Part of the process.
// Next step, draw lines between pixels!!! Bresenham time.
// We're giving up circles. It was fun but bye bye.
// Step 4 done: I've got good interactivity, with even mouse motion.
// Next step is to fix perspective, zooming, and rotating.
// Step after that will be colors. Then, we'll have something 99% good.
// Next we should center the shape at 1st draw (so we can see everything) - this doesn't need to be perfect.
// Next step after that would be hiding lines according to depth.
// With all that done, we should have something pretty much "good enough" I guess, and I can move on.
// Unless I want to do more, like not calculating outside of screen, doing default colors, etc...
// (after cleaning up code completely ofc)
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
