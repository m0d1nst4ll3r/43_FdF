/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:54:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 18:06:42 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	set_hooks(t_fdf *d)
{
	mlx_do_key_autorepeatoff(d->mlx.ptr);
	mlx_hook(d->mlx.win, KEYPRESS, KEYPRESSMASK, key_down_hook, d);
	mlx_hook(d->mlx.win, KEYRELEASE, KEYRELEASEMASK, key_up_hook, &d->key);
	mlx_hook(d->mlx.win, BUTTONPRESS, BUTTONPRESSMASK, mouse_down_hook, d);
	mlx_hook(d->mlx.win, BUTTONRELEASE, BUTTONRELEASEMASK,
		mouse_up_hook, &d->mouse);
	mlx_hook(d->mlx.win, MOTIONNOTIFY, POINTERMOTIONMASK,
		pointer_motion_hook, &d->mouse);
	mlx_hook(d->mlx.win, CLIENTMESSAGE, 1, clientmsg_hook, d);
	mlx_loop_hook(d->mlx.ptr, engine_loop, d);
}
