/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:09:27 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 11:43:13 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Shift to the left to allow greater precision
// FP here means Fixed Point (as opposed to Floating Point)
t_color	decompose_color(int color_int)
{
	t_color	color_rgb;

	color_rgb.r = ((color_int >> 16) & 0xff) << FP_SHIFT;
	color_rgb.g = ((color_int >> 8) & 0xff) << FP_SHIFT;
	color_rgb.b = (color_int & 0xff) << FP_SHIFT;
	return (color_rgb);
}

// Shift back to collapse precision and get approximate color
int	recompose_color(t_color color_rgb)
{
	return ((((color_rgb.r >> FP_SHIFT & 0xff) << 16))
		+ (((color_rgb.g >> FP_SHIFT & 0xff) << 8))
		+ (color_rgb.b >> FP_SHIFT & 0xff));
}

void	add_colors(t_color *c1, t_color c2)
{
	c1->r += c2.r;
	c1->g += c2.g;
	c1->b += c2.b;
}

void	get_color_step(t_color c1, t_color c2, int step,
		t_color *color_step)
{
	if (step > 1)
	{
		color_step->r = (c2.r - c1.r) / (step - 1);
		color_step->g = (c2.g - c1.g) / (step - 1);
		color_step->b = (c2.b - c1.b) / (step - 1);
	}
}
