/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:26 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 15:29:03 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define REFRESH_RATE	16666
# define WIN_X			1920
# define WIN_Y			1080
# define WIN_NAME		"fdf"

# define BRUSH_WID_MAX	255
# define BRUSH_WID_MIN	0

# define ERRMLX			"Error initializing mlx"
# define ERRWIN			"Error initializing window"
# define ERRIMG			"Error initializing image"
# define ERRMAL			"Malloc error"
# define ERROPN			"Error opening map"
# define ERRREA			"Error reading map"
# define ERREMP			"Map is empty"
# define ERRMAP			"Incorrect map format"

# include "mlx.h"
# include "libft.h"
# include "xmacros.h"
# include <string.h>
# include <errno.h>

# include <sys/time.h> // Remove if I don't optimize like this
# include <stdbool.h> // Remove if I don't use bools

typedef struct	s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		llen;
	int		endian;
}	t_img;

typedef struct	s_fdf
{
	char			**av;
	void			*mlx;
	void			*win;
	t_img			img;
	int				lmb_held;
	int				rmb_held;
	bool			refresh_needed;
	struct timeval	old_time;
	struct timeval	cur_time;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	brush_width;
	bool			full_circle;
	bool			invert_draw;
}	t_fdf;

// mlx_util.c
int		pixel_put(t_img img, int x, int y, int color, bool invert);

// init.c
void	init_prog(t_fdf *d, char **av);

// exit.c
void	exit_prog(t_fdf d, unsigned char exitval);
void	error_out(t_fdf d, char *s);

// hook.c
void	set_hooks(t_fdf *d);

#endif
