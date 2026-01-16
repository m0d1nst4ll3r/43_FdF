/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:21:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/16 13:26:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Writes directly into mlx image
// Display does not update until next put_image_to_window
int	pixel_put(t_img img, int x, int y, int color)
{
	char	*addr;

	addr = img.addr + y * img.llen + x * img.bpp / 8;
	if (*(int *)addr != color)
	{
		*(int *)addr = color;
		return (1);
	}
	return (0);
}
