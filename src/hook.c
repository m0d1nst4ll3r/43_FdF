/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:54:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/30 19:41:16 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	move_down(t_fdf *d)
{
	d->y_offset += MOVE_SPEED;
	d->redraw_needed = true;
}

static void	move_up(t_fdf *d)
{
	d->y_offset -= MOVE_SPEED;
	d->redraw_needed = true;
}

static void	move_left(t_fdf *d)
{
	d->x_offset -= MOVE_SPEED;
	d->redraw_needed = true;
}

static void	move_right(t_fdf *d)
{
	d->x_offset += MOVE_SPEED;
	d->redraw_needed = true;
}

static void	shift_down(t_fdf *d)
{
	d->height_mod--;
	d->redraw_needed = true;
}

static void	shift_up(t_fdf *d)
{
	d->height_mod++;
	d->redraw_needed = true;
}

static void	shift_left(t_fdf *d)
{
	d->line_offset--;
	d->redraw_needed = true;
}

static void	shift_right(t_fdf *d)
{
	d->line_offset++;
	d->redraw_needed = true;
}

static void	zoom_in(t_fdf *d)
{
	d->point_distance++;
	d->redraw_needed = true;
}

static void	zoom_out(t_fdf *d)
{
	d->point_distance--;
	d->redraw_needed = true;
}

static int	key_down_hook(int key, t_fdf *d)
{
	if (key == ESC)
		exit_prog(*d, 0);
	if (key == UP) 
		d->key_states.up = true;
	else if (key == DOWN)
		d->key_states.down = true;
	else if (key == LEFT) 
		d->key_states.left = true;
	else if (key == RIGHT)
		d->key_states.right = true;
	else if (key == LSHIFT) 
		d->key_states.lshift = true;
	else if (key == LCTRL)
		d->key_states.lctrl = true;
	else if (key == 'a') 
		d->key_states.a = true;
	else if (key == 'd')
		d->key_states.d = true;
	else if (key == 'r') 
		d->key_states.r = true;
	else if (key == 'f')
		d->key_states.f = true;
	return (0);
}

static int	key_up_hook(int key, t_fdf *d)
{
	if (key == UP) 
		d->key_states.up = false;
	else if (key == DOWN)
		d->key_states.down = false;
	else if (key == LEFT) 
		d->key_states.left = false;
	else if (key == RIGHT)
		d->key_states.right = false;
	else if (key == LSHIFT) 
		d->key_states.lshift = false;
	else if (key == LCTRL)
		d->key_states.lctrl = false;
	else if (key == 'a') 
		d->key_states.a = false;
	else if (key == 'd')
		d->key_states.d = false;
	else if (key == 'r') 
		d->key_states.r = false;
	else if (key == 'f')
		d->key_states.f = false;
	return (0);
}

static int	clientmsg_hook(t_fdf *d)
{
	exit_prog(*d, 0);
	return (0);
}

static void	eraser(t_fdf *d, int x, int y, int width)
{
	int	i;
	int	j;

	i = x - width / 2;
	if (i < 0)
		i = 0;
	while (i <= 1919 && i <= x + width / 2)
	{
		j = y - width / 2;
		if (j < 0)
			j = 0;
		while (j <= 1079 && j <= y + width / 2)
		{
			if (pixel_put(d->img, i, j, 0x000000) && !d->refresh_needed)
				d->refresh_needed = true;
			j++;
		}
		i++;
	}
	
}

static int	pointer_motion_hook(int x, int y, t_fdf *d)
{
	if (x >= 0 && x <= 1919 && y >= 0 && y <= 1079 && (d->lmb_held || d->rmb_held))
	{
		if (d->lmb_held)
		{
			if (pixel_put(d->img, x, y, 0xff0000) && !d->refresh_needed)
				d->refresh_needed = true;
		}
		else
			eraser(d, x, y, 40);
	}
	return (0);
}

static int	mouse_hook(int button, int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	if (button == LMB)
		d->lmb_held = !d->lmb_held;
	else if (button == RMB)
		d->rmb_held = !d->rmb_held;
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

static void	reset_image(t_img img)
{
	int	y;
	int	x;

	y = 0;
	while (y < WIN_Y)
	{
		x = 0;
		while (x < WIN_X)
		{
			pixel_put(img, x, y, 0);
			x++;
		}
		y++;
	}
}

static void	key_states_handler(t_fdf *d)
{
	if (d->key_states.up) 
		move_down(d);
	if (d->key_states.down)
		move_up(d);
	if (d->key_states.left) 
		move_right(d);
	if (d->key_states.right)
		move_left(d);
	if (d->key_states.lshift) 
		zoom_in(d);
	if (d->key_states.lctrl)
		zoom_out(d);
	if (d->key_states.a) 
		shift_left(d);
	if (d->key_states.d)
		shift_right(d);
	if (d->key_states.r) 
		shift_up(d);
	if (d->key_states.f)
		shift_down(d);
	return (0);
}

static int	sync_hook(t_fdf *d)
{
	gettimeofday(&d->cur_time, NULL);
	if (d->redraw_needed)
	{
		reset_image(d->img);
		draw_image(d); // For now this is a very basic function, will improve
		d->redraw_needed = false;
	}
	if (d->refresh_needed && is_time_to_refresh(d->old_time, d->cur_time, REFRESH_RATE))
	{
		d->old_time = d->cur_time;
		mlx_put_image_to_window(d->mlx, d->win, d->img.img, 0, 0);
		d->refresh_needed = false;
		d->frame_count++;
	}
	if (d->fps_time.tv_sec < d->cur_time.tv_sec)
	{
		printf("Fps: %d\n", d->frame_count);
		d->frame_count = 0;
		d->fps_time = d->cur_time;
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
