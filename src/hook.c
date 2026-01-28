/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:54:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 15:11:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	key_hook(int key, t_fdf *d)
{
	if (key == ESC)
		exit_prog(*d, 0);
	else if (key == ' ')
		d->full_circle = !d->full_circle;
	return (0);
}

static int	clientmsg_hook(t_fdf *d)
{
	exit_prog(*d, 0);
	return (0);
}

static int	octant_circle_draw_full(t_img img, int x, int y, int cx, int cy, int color)
{
	int	ret;
	int	lx;

	ret = 0;
	lx = x - cx;
	while (lx <= x + cx)
	{
		ret = pixel_put(img, lx, y + cy, color) || ret;
		ret = pixel_put(img, lx, y - cy, color) || ret;
		lx++;
	}
	lx = x - cy;
	while (lx <= x + cy)
	{
		ret = pixel_put(img, lx, y + cx, color) || ret;
		ret = pixel_put(img, lx, y - cx, color) || ret;
		lx++;
	}
	return (ret);
}

static int	octant_circle_draw(t_img img, int x, int y, int cx, int cy, int color)
{
	int	ret;

	ret = 0;
	ret = pixel_put(img, x + cx, y + cy, color) || ret;
	ret = pixel_put(img, x - cx, y + cy, color) || ret;
	ret = pixel_put(img, x + cx, y - cy, color) || ret;
	ret = pixel_put(img, x - cx, y - cy, color) || ret;
	ret = pixel_put(img, x + cy, y + cx, color) || ret;
	ret = pixel_put(img, x - cy, y + cx, color) || ret;
	ret = pixel_put(img, x + cy, y - cx, color) || ret;
	ret = pixel_put(img, x - cy, y - cx, color) || ret;
	return (ret);
}

// centered on x,y, of radius n
// radius of 0 is just a point
// radius of 1 is 3 pixels wide, 2 is 5, etc..
static int	draw_circle(t_img img, int x, int y, int radius, int color, int full)
{
	int	t1;
	int	t2;
	int	cx;
	int	cy;
	int	ret;

	t1 = radius / 16;
	cx = radius;
	cy = 0;
	ret = 0;
	while (cx >= cy)
	{
		if (!full && octant_circle_draw(img, x, y, cx, cy, color))
			ret = 1;
		else if (full && octant_circle_draw_full(img, x, y, cx, cy, color))
			ret = 1;
		cy++;
		t1 = t1 + cy;
		t2 = t1 - cx;
		if (t2 >= 0)
		{
			t1 = t2;
			cx--;
		}
	}
	return (ret);
}

static int	combine_colors(unsigned char r, unsigned char g, unsigned char b)
{
	return ((r << 16) + (g << 8) + b);
}

static unsigned char	clamp_mod(unsigned char val, unsigned char mod, char sign)
{
	if (sign >= 0)
		return (ft_min(255 - val, mod));
	return (ft_min(val, mod));
}

static void	rotate_colors(t_fdf *d, unsigned char strength)
{
	if (d->b == 255)
	{
		if (d->r)
			d->r -= clamp_mod(d->r, strength, -1);
		else if (d->g != 255)
			d->g += clamp_mod(d->g, strength, 1);
		else
			d->b -= clamp_mod(d->b, strength, -1);
	}
	else if (d->g == 255) 
	{
		if (d->b)
			d->b -= clamp_mod(d->b, strength, -1);
		else if (d->r != 255)
			d->r += clamp_mod(d->r, strength, 1);
		else
			d->g -= clamp_mod(d->g, strength, -1);
	}
	else if (d->r == 255)
	{
		if (d->g)
			d->g -= clamp_mod(d->g, strength, -1);
		else if (d->b != 255)
			d->b += clamp_mod(d->b, strength, 1);
		else
			d->r -= clamp_mod(d->r, strength, -1);
	}
}

static int	pointer_motion_hook(int x, int y, t_fdf *d)
{
	if (d->lmb_held || d->rmb_held)
	{
		if (d->lmb_held)
		{
			if (draw_circle(d->img, x, y, d->brush_width, combine_colors(d->r, d->g, d->b), d->full_circle))
				d->refresh_needed = true;
			rotate_colors(d, 1);
		}
		else
		{
			if (draw_circle(d->img, x, y, d->brush_width, 0, d->full_circle))
				d->refresh_needed = true;
		}
	}
	return (0);
}

static void	increase_brush_width(t_fdf *d)
{
	if (d->brush_width != 255)
		d->brush_width++;
}

static void	decrease_brush_width(t_fdf *d)
{
	if (d->brush_width)
		d->brush_width--;
}

static int	mouse_hook(int button, int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	if (button == LMB)
	{
		d->lmb_held = !d->lmb_held;
		if (draw_circle(d->img, x, y, d->brush_width, combine_colors(d->r, d->g, d->b), d->full_circle))
			d->refresh_needed = true;
		rotate_colors(d, 1);
	}
	else if (button == RMB)
	{
		d->rmb_held = !d->rmb_held;
		if (draw_circle(d->img, x, y, d->brush_width, 0, d->full_circle))
			d->refresh_needed = true;
	}
	else if (button == MWU)
		increase_brush_width(d);
	else if (button == MWD)
		decrease_brush_width(d);
	return (0);
}

static int	is_time_to_refresh(struct timeval old_time, struct timeval cur_time, int usec_to_refresh)
{
	if (old_time.tv_sec == cur_time.tv_sec)
	{
		if (old_time.tv_usec + usec_to_refresh < cur_time.tv_usec)
			return (1);
		return (0);
	}
	if (old_time.tv_sec + 1 == cur_time.tv_sec)
	{
		if (old_time.tv_usec + usec_to_refresh < cur_time.tv_usec+ 1000000)
			return (1);
		return (0);
	}
	if (old_time.tv_sec + 1 < cur_time.tv_sec)
		return (1);
	return (0);
}

static int	sync_hook(t_fdf *d)
{
	gettimeofday(&d->cur_time, NULL);
	if (d->refresh_needed && is_time_to_refresh(d->old_time, d->cur_time, REFRESH_RATE))
	{
		d->old_time = d->cur_time;
		mlx_put_image_to_window(d->mlx, d->win, d->img.img, 0, 0);
		d->refresh_needed = false;
	}
	return (0);
}

void	set_hooks(t_fdf *d)
{
	mlx_hook(d->win, KEYPRESS, KEYPRESSMASK, key_hook, d);
//	mlx_hook(d->win, KEYRELEASE, KEYRELEASEMASK, key_hook, d);
	mlx_hook(d->win, BUTTONPRESS, BUTTONPRESSMASK, mouse_hook, d);
	mlx_hook(d->win, BUTTONRELEASE, BUTTONRELEASEMASK, mouse_hook, d);
	mlx_hook(d->win, MOTIONNOTIFY, POINTERMOTIONMASK, pointer_motion_hook, d);
	mlx_hook(d->win, CLIENTMESSAGE, 1, clientmsg_hook, d);
	mlx_loop_hook(d->mlx, sync_hook, d);
}
