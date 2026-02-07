/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_key_repeat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:47:09 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/07 12:28:10 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// E.g if REPEAT_RATE is 10 ms (100 hz)
// And if since last engine loop, 25 ms passed (because of very high frametime)
// Then keys repeated 2.5 times since last loop iteration
// If there was a leftover 0.6 key repeat times from last time
// Then we consider that keys repeated 3 times, and a leftover of 0.1 carries
//	over to the next loop iteration.
// This should only be useful in case of either very high key repeat frequency
//	or very high frametime.
static int	get_repeat_actions(t_fdf *d)
{
	int	actions;

	if (d->key.repeat.state == REPEAT)
	{
		d->key.repeat.time_ratio += (float)ft_time_diff(d->time.current,
				d->time.last_key_repeat) / KEY_REPEAT_RATE_USEC;
		d->time.last_key_repeat = d->time.current;
		actions = (int)d->key.repeat.time_ratio;
		d->key.repeat.time_ratio -= actions;
		return (actions);
	}
	return (0);
}

// If key is on, execute once, go to repeat
// If key is repeat, execute however many key repeats were simulated
// Return value is used to update key repeat state
static bool	execute_keys(t_fdf *d, int actions)
{
	bool	any_key_on;
	int		i;

	any_key_on = false;
	i = 0;
	while (i < KEY_COUNT)
	{
		if (d->key.states[i] == ON || d->key.states[i] == REPEAT)
		{
			any_key_on = true;
			if (d->key.states[i] == ON)
				d->key.actions[i](d, 1);
			else if (actions)
				d->key.actions[i](d, actions);
			d->key.states[i] = REPEAT;
		}
		i++;
	}
	return (any_key_on);
}

// No keys on?	->	Turn timer off.
// Any keys on?
//	Timer off?		->	Start it anew.
//	Timer on?
//		Is it still running? 	->	Calculate whether it should end
//		Is it already over?		->	Do nothing.
static void	update_repeat_state(t_fdf *d, bool any_key_on)
{
	if (!any_key_on)
		d->key.repeat.state = OFF;
	else if (d->key.repeat.state == OFF)
	{
		d->time.last_key_repeat = d->time.current;
		d->key.repeat.state = ON;
	}
	else if (d->key.repeat.state == ON
		&& ft_time_diff(d->time.current, d->time.last_key_repeat)
		> KEY_REPEAT_DELAY_USEC)
	{
		d->key.repeat.state = REPEAT;
		d->time.last_key_repeat = d->time.current;
		d->key.repeat.time_ratio = 0;
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
//			action triggers thrice
//		- This ratio is saved in its own float, partial completion carries over
//			the next engine loop iteration
void	key_states_handler(t_fdf *d)
{
	int		actions;
	bool	any_key_on;

	actions = get_repeat_actions(d);
	execute_keys(d, actions);
	update_repeat_state(d, any_key_on);
}
