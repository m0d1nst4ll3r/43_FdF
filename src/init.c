/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:53 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 16:05:43 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_key_codes_and_actions(t_key *key)
{
	key->codes[W] = KEY_W;
	key->codes[A] = KEY_A;
	key->codes[S] = KEY_S;
	key->codes[D] = KEY_D;
	key->codes[R] = KEY_R;
	key->codes[F] = KEY_F;
	key->codes[UP] = KEY_UP;
	key->codes[LEFT] = KEY_LEFT;
	key->codes[DOWN] = KEY_DOWN;
	key->codes[RIGHT] = KEY_RIGHT;
	key->codes[LSHIFT] = KEY_LSHIFT;
	key->codes[LCTRL] = KEY_LCTRL;
	key->actions[R] = shift_up;
	key->actions[F] = shift_down;
	key->actions[UP] = move_up;
	key->actions[LEFT] = move_left;
	key->actions[DOWN] = move_down;
	key->actions[RIGHT] = move_right;
	key->actions[LSHIFT] = zoom_in;
	key->actions[LCTRL] = zoom_out;
}

// Zero fill actions to avoid accidental segfaults
//	when adding keys and forgetting to add corresponding actions
static void	init_key(t_key *key)
{
	int	i;

	i = 0;
	while (i < KEY_COUNT)
	{
		key->states[i] = OFF;
		key->actions[i] = 0;
		key->codes[i] = 0;
		i++;
	}
	init_key_codes_and_actions(key);
	key->repeat.state = OFF;
}

static void	init_mouse(t_mouse *mouse)
{
	mouse->lmb_held = false;
	mouse->rmb_held = false;
}

// gettimeofday can fail
static bool	init_time(t_time *time)
{
	time->img_state = IMG_NEED_REDRAW;
	time->frame_count = 0;
	time->loop_count = 0;
	if (gettimeofday(&time->last_refresh, NULL))
		return (false);
	time->last_fps = time->last_refresh;
	return (true);
}

// Values to init
//	- mlx addresses to NULL for now
//	- file
//		- name + future chained list
//		- do not init fd, useless (we get it through open, first failure point)
//	- map
//		- all 3 array addresses
//	- starting states
//	- key/mouse values
//		- do not init repeat ratio, it is set to 0 automatically at repeat start
//		- do not init mouse pos, this is set on false->true lmb/rmb held change
//	- time values
//		- do not init last_key_repeat, it is set on key state ON->REPEAT change
//		- do not init current, it is set on mlx loop
void	init_prog(t_fdf *d, char *filename)
{
	init_mlx_null(&d->mlx);
	init_file(&d->file);
	init_map(&d->map);
	init_state(&d->state);
	init_key(&d->key);
	init_mouse(&d->mouse);
	if (!init_time(&d->time))
	   error_out(d, ERRTIME);
}
