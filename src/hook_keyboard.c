/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_keyboard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:49:36 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 18:34:17 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	set_key_state(int key, t_fdf *d, t_key_state state)
{
	if (key == KEY_UP && (state == OFF || d->key_state[UP] == OFF))
		d->key_state[UP] = state;
	else if (key == KEY_DOWN && (state == OFF || d->key_state[DOWN] == OFF))
		d->key_state[DOWN] = state;
	else if (key == KEY_LEFT && (state == OFF || d->key_state[LEFT] == OFF))
		d->key_state[LEFT] = state;
	else if (key == KEY_RIGHT && (state == OFF || d->key_state[RIGHT] == OFF))
		d->key_state[RIGHT] = state;
	else if (key == KEY_LSHIFT && (state == OFF || d->key_state[LSHIFT] == OFF))
		d->key_state[LSHIFT] = state;
	else if (key == KEY_LCTRL && (state == OFF || d->key_state[LCTRL] == OFF))
		d->key_state[LCTRL] = state;
	else if (key == 'a' && (state == OFF || d->key_state[A] == OFF))
		d->key_state[A] = state;
	else if (key == 'd' && (state == OFF || d->key_state[D] == OFF))
		d->key_state[D] = state;
	else if (key == 'r' && (state == OFF || d->key_state[R] == OFF))
		d->key_state[R] = state;
	else if (key == 'f' && (state == OFF || d->key_state[F] == OFF))
		d->key_state[F] = state;
}

int	key_down_hook(int key, t_fdf *d)
{
	if (key == KEY_ESC)
		exit_prog(*d, 0);
	set_key_state(key, d, ON);
	return (0);
}

int	key_up_hook(int key, t_fdf *d)
{
	set_key_state(key, d, OFF);
	return (0);
}
