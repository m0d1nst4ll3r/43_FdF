/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_mouse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:48:23 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/03 12:26:39 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	pointer_motion_hook(int x, int y, t_fdf *d)
{
	if (d->lmb_held)
	{
		if (x != d->mouse_pos.x)
		{
			move_x(d, x - d->mouse_pos.x);
			d->mouse_pos.x = x;
		}
		if (y != d->mouse_pos.y)
		{
			move_y(d, y - d->mouse_pos.y);
			d->mouse_pos.y = y;
		}
	}
	// TODO: rmg_held (when I have good rotation)
	return (0);
}

int	mouse_down_hook(int button, int x, int y, t_fdf *d)
{
	if (button == BTN_LMB)
	{
		d->mouse_pos.x = x;
		d->mouse_pos.y = y;
		d->lmb_held = true;
	}
	else if (button == BTN_RMB)
	{
		d->mouse_pos.x = x;
		d->mouse_pos.y = y;
		d->rmb_held = true;
	}
	else if (button == BTN_MWU)
		zoom_in(d, 1);
	else if (button == BTN_MWD)
		zoom_out(d, 1);
	return (0);
}

int	mouse_up_hook(int button, int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	if (button == BTN_LMB)
		d->lmb_held = false;
	else if (button == BTN_RMB)
		d->rmb_held = false;
	return (0);
}
