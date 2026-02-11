/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_mouse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:48:23 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/11 19:56:59 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	lmb_handler(int x, int y, t_fdf *d)
{
	if (x != d->mouse.pos.x)
	{
		move_right(d, d->mouse.pos.x - x);
		d->mouse.pos.x = x;
	}
	if (y != d->mouse.pos.y)
	{
		move_down(d, d->mouse.pos.y - y);
		d->mouse.pos.y = y;
	}
}

static void	rmb_handler(int x, int y, t_fdf *d)
{
	if (x != d->mouse.pos.x)
	{
		rotate_z_increase(d, d->mouse.pos.x - x);
		d->mouse.pos.x = x;
	}
	if (y != d->mouse.pos.y)
	{
		rotate_x_increase(d, d->mouse.pos.y - y);
		d->mouse.pos.y = y;
	}
}

int	pointer_motion_hook(int x, int y, t_fdf *d)
{
	if (d->mouse.lmb_held)
		lmb_handler(x, y, d);
	else if (d->mouse.rmb_held)
		rmb_handler(x, y, d);
	return (0);
}

// LMB/RMB enable position tracking used in pointer motion hook
// MWU/MWD act on zoom
int	mouse_down_hook(int button, int x, int y, t_fdf *d)
{
	if (button == BTN_LMB)
	{
		d->mouse.pos.x = x;
		d->mouse.pos.y = y;
		d->mouse.lmb_held = true;
	}
	else if (button == BTN_RMB)
	{
		d->mouse.pos.x = x;
		d->mouse.pos.y = y;
		d->mouse.rmb_held = true;
	}
	else if (button == BTN_MWU)
		zoom_in_mouse(d, x, y);
	else if (button == BTN_MWD)
		zoom_out_mouse(d, x, y);
	return (0);
}

int	mouse_up_hook(int button, int x, int y, t_mouse *mouse)
{
	(void)x;
	(void)y;
	if (button == BTN_LMB)
		mouse->lmb_held = false;
	else if (button == BTN_RMB)
		mouse->rmb_held = false;
	return (0);
}
