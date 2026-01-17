/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:26 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/17 16:10:28 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define REFRESH_RATE	16666
# define WIN_X			1920
# define WIN_Y			1080
# define WIN_NAME		"fdf"

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

# include <time.h> // Remove this
# include <sys/time.h> // Remove if I don't optimize like this
# include <stdbool.h> // Remove if I don't use bools
# include <stdio.h> // Remove, this is for comparing speeds with my ft_printf

// Structure for map array
typedef struct	s_point
{
	int	height;
	int	color;
}	t_point;

// List for reading file
typedef struct	s_file
{
	char			*line;
	struct s_file	*next;
}	t_file;

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
	t_point			*map; // This contains our map data
	t_file			*file; // This contains our file (only useful during program init)
	int				lmb_held; // This isn't very useful for now
	int				rmb_held; // All this is for fun for now
	bool			refresh_needed;
	int				frame_count;
	struct timeval	old_time;
	struct timeval	cur_time;
	struct timeval	fps_time;
}	t_fdf;

// mlx_util.c
int		pixel_put(t_img img, int x, int y, int color);

// init.c
void	init_prog(t_fdf *d, char **av);

// exit.c
void	exit_prog(t_fdf d, unsigned char exitval);
void	error_out(t_fdf d, char *s);

// hook.c
void	set_hooks(t_fdf *d);

#endif
