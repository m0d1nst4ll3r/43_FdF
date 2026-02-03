/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:26 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/03 12:28:54 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

// Framerate limit
// Current 60 hz
# define REFRESH_RATE	16666
// How long to hold the key down for it to start repeating (OS is 500ms)
// Current 250 ms
# define REPEAT_DELAY	250000
// Frequency of key repeats (OS is 33hz)
// Current 33 hz
# define REPEAT_RATE	30000
# define WIN_X			1920
# define WIN_Y			1080
# define WIN_NAME		"fdf"

# define NUM_KEYS		12
# define W			0
# define S			1
# define A			2
# define D			3
# define LSHIFT		4
# define LCTRL		5
# define R			6
# define F			7
# define UP			8
# define DOWN		9
# define LEFT		10
# define RIGHT		11

# define X_OFFSET		500
# define Y_OFFSET		500
# define POINT_DISTANCE	100
# define HEIGHT_MOD		10
# define LINE_OFFSET	20
# define MOVE_SPEED		15

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
# define ERRTIME		"Error using gettimeofday"

# include "mlx.h"
# include "libft.h"
# include "xmacros.h"
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <stdbool.h>

// Keys can be:
// Off		Not pressed
// On		Pressed, next frame will execute movement
// Wait		Pressed, movement was executed, waiting for repeat delay
// Repeat	Pressed, enough time passed so key is now repeating
// Keys move states downwards. Releasing the key resets it to Off.
// If a key has been waiting, new keys will only wait as long as the first one.
typedef enum	e_key_state
{
	OFF,
	ON,
	REPEAT
}	t_key_state;

// Structure for map array
typedef struct	s_point
{
	short	height;
	int		color;
}	t_point;

typedef struct	s_coord
{
	int	x;
	int	y;
}	t_coord;

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
	int				fd; // Has to be here to close it in exit_prog
	int				x_offset; // This is for testing/fun but should be useful later for
	int				y_offset; // -
	int				point_distance; // -
	int				height_mod; // -
	int				line_offset; // -
	t_key_state		key_state[NUM_KEYS];
	t_key_state		key_repeat_state;
	struct timeval	key_repeat_time;
	float			key_repeat_time_ratio;
	bool			lmb_held;
	bool			rmb_held;
	t_coord			mouse_pos;
	bool			refresh_needed; // Note that it should be useful later if I want to implement mouse-dragging to rotate the shape around
	int				frame_count; // In that case, mlx_loop will not only put_image but also draw_image
	struct timeval	old_time; // Draw_image in that case should have a dedicated redraw_needed value that updates on user interaction
	struct timeval	cur_time;
	struct timeval	fps_time;
	bool			redraw_needed; // redraw_needed value I'll need eventually
}	t_fdf;

// mlx_util.c
int				pixel_put(t_img img, int x, int y, int color);

// init.c
void			init_prog(t_fdf *d, char **av);
void			init_mlx(t_fdf *d);
// exit.c
void			exit_prog(t_fdf d, unsigned char exitval);
void			error_out(t_fdf d, char *s);

//	MAP BUILDING
// map_file.c
void			free_file(t_file **file);
unsigned short	read_file(t_fdf *d);
// map_build.c
void			get_map(t_fdf *d);
// map_valid.c
void			get_widths(t_fdf d);

//	EVENT HOOKS
// hook.c
void			set_hooks(t_fdf *d);
// hook_mouse.c
int				pointer_motion_hook(int x, int y, t_fdf *d); // TODO: pointer motion controls
int				mouse_down_hook(int button, int x, int y, t_fdf *d);
int				mouse_up_hook(int button, int x, int y, t_fdf *d);
// hook_keyboard.c
int				key_down_hook(int key, t_fdf *d);
int				key_up_hook(int key, t_fdf *d);

// INTERACTIVE
// interact_translate.c
void			move_x(t_fdf *d, int actions);
void			move_y(t_fdf *d, int actions);
// interact_zoom.c
void			zoom_in(t_fdf *d, int actions); // TODO: zoom is bad
void			zoom_out(t_fdf *d, int actions);
// interact_rotate.c
void			shift_left(t_fdf *d, int actions); // TODO: rotate is bad, improve
void			shift_right(t_fdf *d, int actions); // func names are not even good
// interact_height.c
void			shift_up(t_fdf *d, int actions); // TODO: float instead of int
void			shift_down(t_fdf *d, int actions); // Call this with MWU MWD

// draw.c
void			draw_image(t_fdf *d); // TODO: improve everything, this was done super fast
// TODO: draw calculates points and lines outside of screen - bad

#endif
