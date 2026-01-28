/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:26 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 15:44:39 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define REFRESH_RATE	16666
# define WIN_X			1920
# define WIN_Y			1080
# define WIN_NAME		"fdf"

# define ERRMLX			"Error initializing mlx"
# define ERRWIN			"Error initializing mlx window"
# define ERRIMG			"Error initializing mlx image"
# define ERRMAL			"Malloc error"
# define ERROPN			"Error opening map"
# define ERRREA			"Error reading map"
# define ERRHEI			"Map has too many lines (> 65535)"
# define ERRWID			"Line has too many points (> 65535)"
# define ERRMAP			"Incorrect map format"
# define ERRVAL			"Incorrect map format: values must fit within a short \
(±32767)"

# include "mlx.h"
# include "libft.h"
# include "xmacros.h"
# include <string.h>
# include <errno.h>
# include <fcntl.h>

# include <time.h> // Remove this
# include <sys/time.h> // Remove if I don't optimize like this
# include <stdbool.h> // Remove if I don't use bools
# include <stdio.h> // Remove, this is for comparing speeds with my ft_printf

// Structure for map array
typedef struct	s_point
{
	short	height;
	int		color;
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
	t_point			*map_dat; // This is the 1D map containing all points
	t_point			**map; // This is the 2D map used for browsing 1D map easily
	unsigned short	*map_widths; // 1D width map, contains each line length
	unsigned short	map_height; // Total vertical length of map (# of ints in width[])
	t_file			*file; // This contains read file (only useful during program init)
	int				fd;
	int				lmb_held; // This isn't very useful for now
	int				rmb_held; // All this is for fun for now
	bool			refresh_needed;
	int				frame_count;
	struct timeval	old_time;
	struct timeval	cur_time;
	struct timeval	fps_time;
}	t_fdf;

// mlx_util.c
int				pixel_put(t_img img, int x, int y, int color);

// init.c
void			init_prog(t_fdf *d, char **av);
void			init_mlx(t_fdf *d);

// exit.c
void			exit_prog(t_fdf d, unsigned char exitval);
void			error_out(t_fdf d, char *s);

// hook.c
void			set_hooks(t_fdf *d);

// file.c
void			free_file(t_file **file);
unsigned short	read_file(t_fdf *d);

// map_build.c
void			get_map(t_fdf *d);

// map_valid.c
void			get_widths(t_fdf d);

#endif
