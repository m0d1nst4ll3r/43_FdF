/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:54:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/16 01:38:46 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	key_hook(int key, t_fdf *d)
{
	if (key == ESC)
		exit_prog(*d, 0);
	return (0);
}

static int	clientmsg_hook(t_fdf *d)
{
	exit_prog(*d, 0);
	return (0);
}

static void	eraser(t_fdf *d, int x, int y, int width)
{
	int	i;
	int	j;

	i = x - width / 2;
	if (i < 0)
		i = 0;
	while (i <= x + width / 2)
	{
		j = y - width / 2;
		if (j < 0)
			j = 0;
		while (j <= y + width / 2)
		{
			pixel_put(d->img, i, j, 0x000000);
			j++;
		}
		i++;
	}
	
}

static int	pointer_motion_hook(int x, int y, t_fdf *d)
{
	if (d->lmb_held || d->rmb_held)
	{
		if (d->lmb_held)
			pixel_put(d->img, x, y, 0xff0000);
		else
			eraser(d, x, y, 40);
	}
	return (0);
}

static int	mouse_hook(int button, int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	if (button == LMB)
		d->lmb_held = !d->lmb_held;
	else if (button == RMB)
		d->rmb_held = !d->rmb_held;
	return (0);
}

static int	sync_hook(t_fdf *d)
{
	mlx_put_image_to_window(d->mlx, d->win, d->img.img, 0, 0);
	return (0);
}

void	set_hooks(t_fdf *d)
{
	mlx_hook(d->win, KEYPRESS, KEYPRESSMASK, key_hook, d);
	mlx_hook(d->win, KEYRELEASE, KEYRELEASEMASK, key_hook, d);
	mlx_hook(d->win, BUTTONPRESS, BUTTONPRESSMASK, mouse_hook, d);
	mlx_hook(d->win, BUTTONRELEASE, BUTTONRELEASEMASK, mouse_hook, d);
	mlx_hook(d->win, MOTIONNOTIFY, POINTERMOTIONMASK, pointer_motion_hook, d);
	mlx_hook(d->win, CLIENTMESSAGE, 1, clientmsg_hook, d);
	mlx_loop_hook(d->mlx, sync_hook, d);
}
