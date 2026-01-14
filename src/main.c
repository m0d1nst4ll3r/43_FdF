/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/14 15:40:57 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define WINX	1920
#define WINY	1080
#define WIN		"fdf"

typedef struct	s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		llen;
	int		endian;
}	t_img;

void	pixel_put(t_img img, int x, int y, int color)
{
	*(int *)(img.addr + y * img.llen + x * img.bpp / 8) = color;
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_img	img;
	int		x;
	int		y;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, WINX, WINY, WIN);
	img.img = mlx_new_image(mlx, WINX, WINY);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.llen, &img.endian);
	y = 0;
	while (y < WINY)
	{
		x = 0;
		while (x < WINX)
		{
			pixel_put(img, x, y, 0x556677);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}
