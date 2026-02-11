/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_subfuncs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 01:11:54 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/11 19:39:35 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// I hate the norm. These could be statics in init.c, but noOooOo
static void	init_key_codes(int codes[KEY_COUNT])
{
	codes[Q] = KEY_Q;
	codes[E] = KEY_E;
	codes[W] = KEY_W;
	codes[A] = KEY_A;
	codes[S] = KEY_S;
	codes[D] = KEY_D;
	codes[R] = KEY_R;
	codes[F] = KEY_F;
	codes[UP] = KEY_UP;
	codes[LEFT] = KEY_LEFT;
	codes[DOWN] = KEY_DOWN;
	codes[RIGHT] = KEY_RIGHT;
	codes[LSHIFT] = KEY_LSHIFT;
	codes[LCTRL] = KEY_LCTRL;
}

static void	init_key_actions(t_key_action actions[KEY_COUNT])
{
	actions[R] = shift_up;
	actions[F] = shift_down;
	actions[Q] = rotate_y_increase;
	actions[E] = rotate_y_decrease;
	actions[W] = rotate_x_increase;
	actions[S] = rotate_x_decrease;
	actions[A] = rotate_z_increase;
	actions[D] = rotate_z_decrease;
	actions[UP] = move_up;
	actions[LEFT] = move_left;
	actions[DOWN] = move_down;
	actions[RIGHT] = move_right;
	actions[LSHIFT] = zoom_in;
	actions[LCTRL] = zoom_out;
}

// Zero fill actions to avoid accidental segfaults when adding keys
//	and forgetting to add corresponding actions
void	init_key(t_key *key)
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
	init_key_codes(key->codes);
	init_key_actions(key->actions);
	key->repeat.state = OFF;
}

void	init_mouse(t_mouse *mouse)
{
	mouse->lmb_held = false;
	mouse->rmb_held = false;
}

// gettimeofday can fail
bool	init_time(t_time *time)
{
	time->img_need_redraw = true;
	time->frame_count = 0;
	time->loop_count = 0;
	if (gettimeofday(&time->last_refresh, NULL))
		return (false);
	time->last_fps = time->last_refresh;
	return (true);
}

