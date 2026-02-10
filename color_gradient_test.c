#include <stdio.h>
#include "mlx.h"
#include "libft.h"
#define WIN_X		2001
#define WIN_Y		250
#define WIN_NAME	"test"
#define FP_SHIFT	16

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
	int	r;
	int	g;
	int	b;
}	t_color;

void	init_mlx(t_mlx *mlx)
{
	mlx->ptr = mlx_init();
	mlx->win = mlx_new_window(mlx->ptr, WIN_X, WIN_Y, WIN_NAME);
	mlx->img.ptr = mlx_new_image(mlx->ptr, WIN_X, WIN_Y);
	mlx->img.addr = mlx_get_data_addr(mlx->img.ptr, &mlx->img.bpp, &mlx->img.llen, &mlx->img.endian);
}

void	pixel_put(t_img *img, int x, int y, int color)
{
	char	*addr;

	if (x < 0 || y < 0 || x >= WIN_X || y >= WIN_Y)
		return ;
	addr = img->addr + y * img->llen + x * img->bpp / 8;
	*(int *)addr = color;
}

int	get_r(int color)
{
	return ((color >> 16) & 0xff);
}

int	get_g(int color)
{
	return ((color >> 8) & 0xff);
}

int	get_b(int color)
{
	return (color & 0xff);
}

int	get_rgb(int r, int g, int b)
{
	return (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

int	main(int ac, char **av)
{
	int		c1;
	int		c2;
	int		i;
	int		j;
	t_mlx	mlx;
	t_color	ci;
	t_color	cstep;

	if (ac != 3)
	{
		c1 = 0xff0000;
		c2 = 0x0000ff;
	}
	else
	{
		ft_atox(av[1], BASE16, &c1, sizeof(c1) | ATOX_LAX);
		ft_atox(av[2], BASE16, &c2, sizeof(c2) | ATOX_LAX);
	}
	init_mlx(&mlx);
	ci.r = get_r(c1) << FP_SHIFT;
	ci.g = get_g(c1) << FP_SHIFT;
	ci.b = get_b(c1) << FP_SHIFT;
	cstep.r = ((get_r(c2) - get_r(c1)) << FP_SHIFT) / (WIN_X - 1);
	cstep.g = ((get_g(c2) - get_g(c1)) << FP_SHIFT) / (WIN_X - 1);
	cstep.b = ((get_b(c2) - get_b(c1)) << FP_SHIFT) / (WIN_X - 1);
	i = 0;
	while (i < WIN_X)
	{
		printf("%d:  rgb  technique: %d\n", i, get_rgb(ci.r >> FP_SHIFT, ci.g >> FP_SHIFT, ci.b >> FP_SHIFT));
		j = 0;
		while (j < WIN_Y)
		{
			pixel_put(&mlx.img, i, j, get_rgb(ci.r >> FP_SHIFT, ci.g >> FP_SHIFT, ci.b >> FP_SHIFT));
			j++;
		}
		ci.r += cstep.r;
		ci.g += cstep.g;
		ci.b += cstep.b;
		i++;
	}
	mlx_put_image_to_window(mlx.ptr, mlx.win, mlx.img.ptr, 0, 0);
	mlx_loop(mlx.ptr);
}
