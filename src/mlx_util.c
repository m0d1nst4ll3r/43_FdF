/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:21:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/10 20:41:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Writes directly into mlx image
// Display does not update until next put_image_to_window
// Only returns true if pixel was actually changed
void	pixel_put(t_img *img, t_point point, int color)
{
	char	*addr;

	if (point.x < 0 || point.y < 0 || point.x >= WIN_X || point.y >= WIN_Y)
		return ;
	addr = img->addr + point.y * img->llen + point.x * img->bpp / 8;
	*(int *)addr = color;
}
