/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_engine_loop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:09:20 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 17:47:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	show_fps(t_time *time)
{
	ft_printf("Engine speed: %d\nFps: %d\n",
		time->loop_count, time->frame_count);
	time->loop_count = 0;
	time->frame_count = 0;
	time->last_fps = time->current;
}

static void	redraw_img(t_fdf *d)
{
	bool	need_refresh;

	need_refresh = false;
	if (reset_image(&d->img))
		need_refresh = true;
	if (draw_image(d))
		need_refresh = true;
	if (need_refresh)
		d->time.img_state = IMG_NEED_REFRESH;
	else
		d->time.img_state = IMG_IDLE;
}

static void	refresh_img(t_fdf *d)
{
	d->time.last_refresh = d->time.current;
	mlx_put_image_to_window(d->mlx.ptr, d->mlx.win, d->mlx.img.ptr, 0, 0);
	d->time.frame_count++;
	d->time.img_state = IMG_IDLE;
}

// Engine loop
//	- Update key repeat timer
//		- If repeat timer is active and enough time's passed, keys will now
//			repeat
//	- Execute actions based on key states
//		- If key is ON, execute action once, move to REPEAT
//		- Else if key is REPEAT and keys are repeating (enough time has passed),
//			execute action however many times the key has repeated since last
//			engine loop iteration (we are using a float)
//		- If any key moved to REPEAT, repeat timer starts (if it wasn't running)
//		- If no key is REPEAT, repeat timer stops
//	- If image needs redraw (any user interaction), redraw
//	- If redraw resulted in any change (very likely), refresh
//	- If we want to show FPS, print FPS
int	engine_loop(t_fdf *d)
{
	if (gettimeofday(&d->time.current, NULL))
	   error_out(d, ERRTIME);
	d->time.loop_count++;
	key_states_handler(d);
	if (d->time.img_state == IMG_NEED_REDRAW)
		redraw_img(d);
	if (d->time.img_state == IMG_NEED_REFRESH
		&& ft_time_diff(d->time.current, d->time.last_refresh) > REFRESH_RATE)
		refresh_img(d);
	if (SHOW_FPS && ft_time_diff(d->time.current, d->time.last_fps) > 1000000)
		show_fps(&d->time);
	return (0);
}
