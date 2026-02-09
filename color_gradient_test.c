#include <stdio.h>
#include "mlx.h"
#define WIN_X		100
#define WIN_Y		100
#define WIN_NAME	"test"

typedef struct	s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		llen;
	int		endian;
}	t_img;

typedef struct	s_mlx
{
	void	*ptr;
	void	*win;
	t_img	img;
}	t_mlx;

typedef struct	s_color
{
	short	r;
	short	g;
	short	b;
}	t_color;

void	init_mlx(t_mlx *mlx)
{
	mlx->ptr = mlx_init();
	mlx->win = mlx_new_window(mlx->ptr, WIN_X, WIN_Y, WIN_NAME);
	mlx->img.ptr = mlx_new_image(mlx->ptr, WIN_X, WIN_Y);
	mlx->img.addr = mlx_get_data_addr(mlx->img.ptr, &mlx->img.bpp, &mlx->img.llen, &mlx->img.endian);
}

t_color	decompose_color(int color_int)
{
	t_color	color_rgb;

	color_rgb.r = (unsigned char)(color_int >> 16);
	color_rgb.g = (unsigned char)(color_int >> 8);
	color_rgb.b = (unsigned char)color_int;
	return (color_rgb);
}

t_color	add_colors(t_color c1, t_color c2)
{
	c1.r += c2.r;
	c1.g += c2.g;
	c1.b += c2.b;
	if (c1.r < 0)
		c1.r = 0;
	else if (c1.r > (unsigned char)-1)
		c1.r = (unsigned char)-1;
	if (c1.g < 0)
		c1.g = 0;
	else if (c1.g > (unsigned char)-1)
		c1.g = (unsigned char)-1;
	if (c1.b < 0)
		c1.b = 0;
	else if (c1.b > (unsigned char)-1)
		c1.b = (unsigned char)-1;
	return (c1);
}

// c1 - c2
// Warning: colors can be negative
t_color	sub_colors(t_color c1, t_color c2)
{
	c1.r -= c2.r;
	c1.g -= c2.g;
	c1.b -= c2.b;
	return (c1);
}

t_color	divide_color(t_color c, int d)
{
	c.r /= d;
	c.g /= d;
	c.b /= d;
	return (c);
}

int	recompose_color(t_color color_rgb)
{
	return (((int)color_rgb.r << 16) + ((int)color_rgb.g << 8) + (int)color_rgb.b);
}

void	pixel_put(t_img *img, int x, int y, int color)
{
	char	*addr;

	if (x < 0 || y < 0 || x >= WIN_X || y >= WIN_Y)
		return ;
	addr = img->addr + y * img->llen + x * img->bpp / 8;
	*(int *)addr = color;
}

int	main()
{
	int		c1;
	int		c2;
	int		ci;
	int		cstep;
	t_color	rgbi;
	t_color	rgbi_last;
	t_color	rgbstep;
	int		i;
	t_mlx	mlx;

	init_mlx(&mlx);
	c1 = 0xff00ff;
	c2 = 0;
	i = 0;
	ci = c1;
	cstep = (c2 - c1) / 80;
	rgbi = decompose_color(c1);
	rgbstep = divide_color(sub_colors(decompose_color(c2), rgbi), 80);
	while (i < 86)
	{
		printf("%d: delta technique: %d, diff %d\n", i, ci, cstep);
		if (!i)
			printf("%d:  rgb  technique: %d, diff %d\n", i, recompose_color(rgbi), 0);
		else
			printf("%d:  rgb  technique: %d, diff %d\n", i, recompose_color(rgbi), recompose_color(rgbi) - recompose_color(rgbi_last));
		pixel_put(&mlx.img, i, 0, ci);
		pixel_put(&mlx.img, i, 1, ci);
		pixel_put(&mlx.img, i, 2, ci);
		pixel_put(&mlx.img, i, 3, ci);
		pixel_put(&mlx.img, i, 4, ci);
		pixel_put(&mlx.img, i, 10, recompose_color(rgbi));
		pixel_put(&mlx.img, i, 11, recompose_color(rgbi));
		pixel_put(&mlx.img, i, 12, recompose_color(rgbi));
		pixel_put(&mlx.img, i, 13, recompose_color(rgbi));
		pixel_put(&mlx.img, i, 14, recompose_color(rgbi));
		ci += cstep;
		rgbi_last = rgbi;
		rgbi = add_colors(rgbi, rgbstep);
		i++;
	}
	mlx_put_image_to_window(mlx.ptr, mlx.win, mlx.img.ptr, 0, 0);
	mlx_loop(mlx.ptr);
}
