/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_mouse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:48:23 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 17:37:42 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	pointer_motion_hook(int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	(void)d;
	return (0);
}

int	mouse_down_hook(int button, int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	if (button == BTN_LMB)
		d->lmb_held = true;
	else if (button == BTN_RMB)
		d->rmb_held = true;
	return (0);
}

int	mouse_up_hook(int button, int x, int y, t_fdf *d)
{
	(void)x;
	(void)y;
	if (button == BTN_LMB)
		d->lmb_held = true;
	else if (button == BTN_RMB)
		d->rmb_held = true;
	return (0);
}
