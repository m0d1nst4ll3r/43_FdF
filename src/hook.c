/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:54:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 17:47:55 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	set_hooks(t_fdf *d)
{
	mlx_do_key_autorepeatoff(d->mlx);
	mlx_hook(d->win, KEYPRESS, KEYPRESSMASK, key_down_hook, d);
	mlx_hook(d->win, KEYRELEASE, KEYRELEASEMASK, key_up_hook, &d->key);
	mlx_hook(d->win, BUTTONPRESS, BUTTONPRESSMASK, mouse_down_hook, d);
	mlx_hook(d->win, BUTTONRELEASE, BUTTONRELEASEMASK,
		mouse_up_hook, &d->mouse);
	mlx_hook(d->win, MOTIONNOTIFY, POINTERMOTIONMASK,
		pointer_motion_hook, &d->mouse);
	mlx_hook(d->win, CLIENTMESSAGE, 1, clientmsg_hook, d);
	mlx_loop_hook(d->mlx, engine_loop, d);
}
