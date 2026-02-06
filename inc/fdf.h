/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:26 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 19:59:45 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

// Window and drawing constants
# define WIN_NAME			"fdf"
# define WIN_X				1920
# define WIN_Y				1080
# define REFRESH_RATE_USEC	16666
# define SHOW_FPS			1
# define POINT_DISTANCE		100

// Interactivity constants
# define KEY_REPEAT_DELAY_USEC	250000
# define KEY_REPEAT_RATE_USEC	30000
# define MOVE_SPEED				15
# define ZOOM_SPEED				0.1
# define ANGLE_MOVE				0.01
# define HEIGHT_MOVE			0.1
# define HEIGHT_MIN				0.2

// Starting states
# define DEFAULT_X_OFFSET	500
# define DEFAULT_Y_OFFSET	500
# define DEFAULT_HEIGHT_MOD	1
# define DEFAULT_ZOOM		1
# define DEFAULT_ANGLE		0.523599

// Map building constants
# define DEFAULT_COLOR		0xffffff
# define BACKGROUND_COLOR	0x0

// Error strings
# define DEFAULT_ERR	"Undefined error"
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
# include <math.h>

// Documents whether to redraw/refresh the image in engine loop
// - Redraw when engine state changes (e.g user pressed KEY_LEFT)
// - Refresh when redraw resulted in actual changes in img
typedef enum	e_img_state
{
	IMG_IDLE,
	IMG_NEED_REDRAW,
	IMG_NEED_REFRESH
}	t_img_state;

// Used for key states
// - Off		Not pressed
// - On			Pressed, next frame will execute movement and go to repeat
// - Repeat		First movement was executed, now waiting for repeat timer
// Keys move states downwards. Releasing the key resets it to Off.
// Repeat timer is handled separately.
typedef enum	e_key_state
{
	OFF,
	ON,
	REPEAT
}	t_key_state;

// These enum values are used as indexes into key codes[] and key actions[]
enum	e_keys
{
	W,
	A,
	S,
	D,
	R,
	F,
	UP,
	LEFT,
	DOWN,
	RIGHT,
	LSHIFT,
	LCTRL,
	KEY_COUNT
};

// Points in the map array (read from map file)
// Their x,y values are inferred from their positions in the array
typedef struct	s_map_point
{
	short	z;
	int		color;
}	t_map_point;

// Simple point structure for passing around points in graph functions
typedef struct	s_point
{
	int	x;
	int	y;
}	t_point;

// This chained list contains file line by line (only used in map building)
typedef struct	s_file_contents
{
	char					*line;
	struct s_file_contents	*next;
}	t_file_contents;

// Mlx img object
// - bypp	bytes per pixel (bpp / 8), in case compiler does not optimize
//			the redundant / 8 operation made on each pixel_put
typedef struct	s_img
{
	void		*ptr;
	char		*addr;
	int			bpp;
	int			bypp;
	int			llen;
	int			endian;
}	t_img;

// Mlx object
typedef struct s_mlx
{
	void	*ptr;
	void	*win;
	t_img	img;
}	t_mlx;

// All map-related data
// - data			1D map containing all points
// - index		2D indexed map
// - widths		line lengths
// - height		map height
// Allocated in get_map() - map_build.c
typedef struct s_map
{
	t_map_point		*data;
	t_map_point		**index;
	unsigned short	height;
	unsigned short	*widths;
}	t_map;

// States used in drawing, modifiable through keybinds
// - x_offset		where to start drawing horizontally
// - y_offset		where to start drawing vertically
// - height_mod		modifier to apply to height values
// - angle			angle value for 3D drawing
//					(this needs more work to achieve better 3D)
typedef struct s_state
{
	int		x_offset;
	int		y_offset;
	float	height_mod;
	float	angle;
	float	zoom;
}	t_state;

// File information used in map building
typedef struct s_file
{
	char			*name;
	int				fd;
	t_file_contents	*contents;
}	t_file;

// Key repeat data
// - time_ratio		# of times keys repeated since last engine loop iteration
// - state			repeat state (off, wait, repeat)
// Used for replacing OS key repeat functionality
typedef struct s_repeat
{
	float		time_ratio;
	t_key_state	state;
}	t_repeat;

// Key bindings and related actions
// - repeat		all data about key repeat
// - states		state of key (off, on, repeat)
// - codes		array of physical key codes
// - actions		corresponding array of actions
// Used for keyboard interactions
typedef struct s_key
{
	t_repeat	repeat;
	t_key_state	states[KEY_COUNT];
	int			codes[KEY_COUNT];
	void		(*actions[KEY_COUNT])(void *, int);
}	t_key;

// Mouse data
// - lmb_held	is lmb held down
// - rmb_held	is rmb held down
// - pos		last known x,y mouse pos from motion notify
// Used for mouse dragging functionality
typedef struct s_mouse
{
	bool	lmb_held;
	bool	rmb_held;
	t_point	pos;
}	t_mouse;

// Time values used in engine loop
// - current			current time (updated at engine loop)
// - refresh			last frame (to sync with refresh rate)
// - fps				last fps print (every 1 second)
// - key_repeat	last	key repeat (to sync with repeat rate)
// - frame_count		for displaying fps
// - loop_count			for displaying engine loops per second
// - img_state			if img needs redrawing or refreshing
typedef struct s_time
{
	struct timeval	current;
	struct timeval	last_refresh;
	struct timeval	last_fps;
	struct timeval	last_key_repeat;
	unsigned int	frame_count;
	unsigned int	loop_count;
	t_img_state		img_state;
}	t_time;

// God struct
// - mlx	All minilibx data
// - file	File data used in parsing/building map
// - map	Map data (what we draw)
// - state	Engine states (how he draw it)
// - key	Key data (for key handlers)
// - mouse	Mouse data (for mouse/pointer handlers)
// - time	Time values (for syncing and time-sensitive calculations)
typedef struct	s_fdf
{
	t_mlx	mlx;
	t_file	file;
	t_map	map;
	t_state	state;
	t_key	key;
	t_mouse	mouse;
	t_time	time;
}	t_fdf;

// mlx_util.c
bool			pixel_put(t_img *img, t_point point, int color);

//	INIT/EXIT/ERROR
// init.c
void			init_prog(t_fdf *d, char *filename);
// init_subfuncs.c
void			init_mlx_null(t_mlx *mlx);
void			init_file(t_file *file, char *filename);
void			init_map(t_map *map);
void			init_state(t_state *state);
// init_mlx.c
void			init_mlx(t_fdf *d);
// exit.c
void			exit_prog(t_fdf *d, unsigned char exitval);
// error.c
void			error_out(t_fdf *d, char *err_str);

//	MAP BUILDING/FILE READING
// map_build.c
void			get_map(t_fdf *d);
// map_file.c
void			free_file(t_file_contents **file);
unsigned short	read_file(t_fdf *d);
// map_valid.c
void			get_widths(t_fdf d);

//	EVENT HOOKS/ENGINE LOOP
// hook.c
void			set_hooks(t_fdf *d);
// hook_engine_loop.c
int				engine_loop(t_fdf *d);
// hook_key_repeat.c
void			key_states_handler(t_fdf *d);
// hook_mouse.c
int				pointer_motion_hook(int x, int y, t_fdf *d);
int				mouse_down_hook(int button, int x, int y, t_fdf *d);
int				mouse_up_hook(int button, int x, int y, t_fdf *d);
// hook_keyboard.c
int				key_down_hook(int key, t_fdf *d);
int				key_up_hook(int key, t_fdf *d);
// hook_window_button.c
int				clientmsg_hook(t_fdf *d);

// INTERACTIVE
// interact_translate.c
void			move_right(t_fdf *d, int actions);
void			move_left(t_fdf *d, int actions);
void			move_up(t_fdf *d, int actions);
void			move_down(t_fdf *d, int actions);
// interact_zoom.c
void			zoom_in(t_fdf *d, int actions); // TODO: zoom is bad
void			zoom_out(t_fdf *d, int actions);
// interact_rotate.c
void			shift_left(t_fdf *d, int actions); // TODO: rotate is bad, improve
void			shift_right(t_fdf *d, int actions); // rename funcs to rotate_x _y
// interact_height.c
void			shift_up(t_fdf *d, int actions);
void			shift_down(t_fdf *d, int actions);

// draw.c
bool			reset_image(t_img *img);
bool			draw_image(t_fdf *d);
// TODO: improve everything, this was done super fast
// TODO: draw calculates points and lines outside of screen - bad

#endif
