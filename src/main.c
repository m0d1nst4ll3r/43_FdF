/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/30 18:02:12 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	display_map_to_make_sure_i_didnt_fuck_it_up(t_fdf data)
{
	int	x;
	int	y;

/*	y = 0;
	ft_printf("Map has %d lines\nLine lengths:\n", data.map_height);
	while (y < data.map_height)
	{
		ft_printf("Line %2d is %3d points long\n", y, data.map_widths[y]);
		y++;
	}*/
	y = 0;
	while (y < data.map_height)
	{
		x = 0;
		while (x < data.map_widths[y])
		{
			if (data.map[y][x].color == -1)
				ft_printf("%hd ", data.map[y][x].height);
			else
				ft_printf("%hd,%#x ", data.map[y][x].height, data.map[y][x].color);
			x++;
		}
		ft_printf("\n");
		y++;
	}
}

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

//	Steps:
// 0. Notes
//		- This is WIP and some artifacts from earlier testing were left behind
//		- For now hooks are WIP, lmb_held/rmb_held, frame related vars
//			and time related vars are temporary/WIP (so ignore them)
// 1. Init prog
//		a. Init all struct address values to NULL in case of exit, set av
//		(we do not init MLX because program might fail at map-building)
//		(init MLX = window pops up = not a good look if we can't even read map)
// 2. Read file and build map
//		a. Try to open it, try to read it, infer map_height
//		(read = fill a chained list, each node is a line, only parse file once)
//		(an empty map is valid)
//		b. Check each line for its valid format and width, write map_widths
//		(lines may have different lengths or be empty)
//		c. Malloc map (we had to wait since it has to be 1 big block)
//		d. Fill map (this might still fail, this is where we check for overflow)
// 3. Interpret map to draw our first picture
//		- At this point the project would be done without bonuses
//		- For now we will interpret the map in a very simplistic way
//
int	main(int ac, char **av)
{
	t_fdf	data;

	if (ac != 2)
		return (0);
	init_prog(&data, av);
	get_map(&data);
	display_map_to_make_sure_i_didnt_fuck_it_up(data);
	init_mlx(&data);
	set_hooks(&data);
	mlx_loop(data.mlx);
	exit_prog(data, 0);
}
