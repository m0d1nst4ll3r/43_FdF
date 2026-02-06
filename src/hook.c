/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:54:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 00:04:52 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	clientmsg_hook(t_fdf *d)
{
	exit_prog(d, 0);
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

// States:
// - OFF
// Key is off, nothing to do
// - ON
// Key is on, need to execute its function once, then move to REPEAT
// - REPEAT (& repeat on)
// Key repeats at desired hz
// - REPEAT (& repeat off)
// Waiting for repeat to be on
//
// Repeat on/off logic:
// When no key is pressed, repeat is OFF
// When any key is ON and swaps to REPEAT, repeat is ON (waiting)
// Key repeat timer is set to current time
// Whenever current time - key repeat time is higher than FREQ
// Repeat is now REPEAT (on)
// Whenever no keys are pressed, repeat goes back to OFF
//
// Whenever enough time has passed during repeat, keys with REPEAT will repeat
// And the timer will update
static int	get_repeat_actions(t_fdf *d)
{
	int	actions;

	if (d->key_repeat_state == REPEAT)
	{
		d->key_repeat_time_ratio += (float)ft_time_diff(d->cur_time,
				d->key_repeat_time) / REPEAT_RATE;
		actions = (int)d->key_repeat_time_ratio;
		d->key_repeat_time_ratio -= (float)actions;
		d->key_repeat_time = d->cur_time;
		return (actions);
	}
	return (0);
}

static void	execute_keys(t_fdf *d, bool r, int a)
{
	if (d->key_state[UP] == ON || (d->key_state[UP] == REPEAT && r))
		move_y(d, MOVE_SPEED * a);
	if (d->key_state[DOWN] == ON || (d->key_state[DOWN] == REPEAT && r))
		move_y(d, - (MOVE_SPEED * a));
	if (d->key_state[LEFT] == ON || (d->key_state[LEFT] == REPEAT && r))
		move_x(d, MOVE_SPEED * a);
	if (d->key_state[RIGHT] == ON || (d->key_state[RIGHT] == REPEAT && r))
		move_x(d, - (MOVE_SPEED * a));
	if (d->key_state[LSHIFT] == ON || (d->key_state[LSHIFT] == REPEAT && r))
		zoom_in(d, a);
	if (d->key_state[LCTRL] == ON || (d->key_state[LCTRL] == REPEAT && r))
		zoom_out(d, a);
	if (d->key_state[A] == ON || (d->key_state[A] == REPEAT && r))
		shift_left(d, a);
	if (d->key_state[D] == ON || (d->key_state[D] == REPEAT && r))
		shift_right(d, a);
	if (d->key_state[R] == ON || (d->key_state[R] == REPEAT && r))
		shift_up(d, a);
	if (d->key_state[F] == ON || (d->key_state[F] == REPEAT && r))
		shift_down(d, a);
}

static void	update_repeat_state(t_fdf *d)
{
	int		i;
	bool	any_on;

	i = 0;
	any_on = false;
	while (i < NUM_KEYS)
	{
		if (d->key_state[i] == ON || d->key_state[i] == REPEAT)
		{
			any_on = true;
			d->key_state[i] = REPEAT;
		}
		i++;
	}
	if (!any_on)
		d->key_repeat_state = OFF;
	else if (any_on && d->key_repeat_state == OFF)
	{
		d->key_repeat_time = d->cur_time;
		d->key_repeat_state = ON;
	}
	else if (d->key_repeat_state == ON && ft_time_diff(d->cur_time, d->key_repeat_time) > REPEAT_DELAY)
	{
		d->key_repeat_state = REPEAT;
		d->key_repeat_time = d->cur_time;
		d->key_repeat_time_ratio = 0;
	}
}

static void	key_states_handler(t_fdf *d)
{
	int		actions;
	bool	repeat;

	actions = get_repeat_actions(d);
	repeat = true;
	if (!actions)
	{
		repeat = false;
		actions = 1;
	}
	execute_keys(d, repeat, actions);
	update_repeat_state(d);
}

static void	show_fps(t_fdf *d)
{
	ft_printf("Engine speed: %d\nFps: %d\n",
		d->time.loop_count, d->time.frame_count);
	d->time.loop_count = 0;
	d->time.frame_count = 0;
	d->time.last_fps = d->time.current;
}

static int	engine_loop(t_fdf *d)
{
	d->time.loop_count++;
	if (gettimeofday(&d->cur_time, NULL))
	   error_out(*d, ERRTIME);
	key_states_handler(d);
	if (d->redraw_needed)
	{
		reset_image(d->img);
		draw_image(d); // For now this is a very basic function, will improve
		d->redraw_needed = false;
	}
	if (d->refresh_needed && is_time_to_refresh(d->old_time, d->cur_time, REFRESH_RATE))
	{
		d->old_time = d->cur_time;
		mlx_put_image_to_window(d->mlx, d->win, d->img.ptr, 0, 0);
		d->refresh_needed = false;
		d->frame_count++;
	}
	if (SHOW_FPS && ft_time_diff(d->cur_time, d->fps_time) > 1000000)
		show_fps(d);
	return (0);
}

void	set_hooks(t_fdf *d)
{
	mlx_do_key_autorepeatoff(d->mlx);
	mlx_hook(d->win, KEYPRESS, KEYPRESSMASK, key_down_hook, d);
	mlx_hook(d->win, KEYRELEASE, KEYRELEASEMASK, key_up_hook, d);
	mlx_hook(d->win, BUTTONPRESS, BUTTONPRESSMASK, mouse_down_hook, d);
	mlx_hook(d->win, BUTTONRELEASE, BUTTONRELEASEMASK, mouse_up_hook, d);
	mlx_hook(d->win, MOTIONNOTIFY, POINTERMOTIONMASK, pointer_motion_hook, d);
	mlx_hook(d->win, CLIENTMESSAGE, 1, clientmsg_hook, d);
	mlx_loop_hook(d->mlx, engine_loop, d);
}
