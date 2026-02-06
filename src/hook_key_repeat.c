/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_key_repeat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:47:09 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 18:01:50 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

// This contains all the logic about key states and keys repeating
//	- Keys can be
//		- OFF
//		- ON
//		- REPEAT
//	- When a key is pressed (held down), it switches to ON
//	- When a key is released, it switches to OFF
//		- This happens in keyboard down/up hooks
//	- On this function call, any ON key causes its action to execute,
//		and goes to REPEAT
//	- If any key is REPEAT, the repeat timer starts (if it wasn't running)
//	- If no keys are REPEAT, the repeat timer stops
//	- Once the timer has run its course, on this function call, any REPEAT
//		key will cause its action to execute a certain amount of times
//	- The amount of times a REPEAT key fires its action depends on the age
//		of the timer compared to the desired KEY_REPEAT_RATE.
//		- E.g if KEY_REPEAT_RATE * 3 time passed since last engine loop, the
//			action triggers thrice.
//		- This ratio is saved in its own float, partial completion carries over
//			the next engine loop iteration.
void	key_states_handler(t_fdf *d)
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
