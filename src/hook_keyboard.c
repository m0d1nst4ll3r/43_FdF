/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_keyboard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:49:36 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 17:34:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	set_key_state(int keycode, t_key *key, t_key_state state)
{
	int	i;

	i = 0;
	while (i < KEY_COUNT)
	{
		if (key->codes[i] == keycode)
		{
			key->states[i] = state;
			return ;
		}
		i++;
	}
}

// For key down/release, we just set the key's state to OFF/ON
// Actions and repeat are handled in the engine loop
int	key_down_hook(int keycode, t_fdf *d)
{
	if (key == KEY_ESC)
		exit_prog(d, 0);
	set_key_state(keycode, &d->key, ON);
	return (0);
}

int	key_up_hook(int keycode, t_key *key)
{
	set_key_state(keycode, key, OFF);
	return (0);
}
