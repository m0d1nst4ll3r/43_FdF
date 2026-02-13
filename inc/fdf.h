/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:26 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/13 16:55:23 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

// Window and drawing constants
# define WIN_NAME			"fdf"
# define WIN_X				1920
# define WIN_Y				1080
# define REFRESH_RATE_USEC	16666
# define SHOW_FPS			0
# define POINT_DISTANCE		100000

// Interactivity constants
# define KEY_REPEAT_DELAY_USEC	150000
# define KEY_REPEAT_RATE_USEC	15000
# define MOVE_SPEED				5
# define ZOOM_SPEED				0.0150
# define ANGLE_MOVE				0.0150
# define HEIGHT_MOVE			0.0150
# define HEIGHT_MIN				0.1

// Starting states
# define DEFAULT_HEIGHT_MOD	0.1
# define DEFAULT_ANGLE_X	0.61548
# define DEFAULT_ANGLE_Y	0.52359
# define DEFAULT_ANGLE_Z	-0.61548

// Map building constants
# define DEFAULT_COLOR		0xffffff
# define BACKGROUND_COLOR	0x0

// Fixed-point precision for color gradients
# define FP_SHIFT			16

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

// Used for key states
// - Off		Not pressed
// - On			Pressed, next frame will execute movement and go to repeat
// - Repeat		First movement was executed, now waiting for repeat timer
// Keys move states downwards. Releasing the key resets it to Off.
// Repeat timer is handled separately.
typedef enum e_key_state
{
	OFF,
	ON,
	REPEAT
}	t_key_state;

// These enum values are used as indexes into key codes[] and key actions[]
enum	e_keys
{
	Q,
	E,
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
	ONE,
	TWO,
	THREE,
	FOUR,
	KEY_COUNT
};

// Forward t_fdf declaration for t_key_action
typedef struct s_fdf	t_fdf;
typedef void			(*t_key_action)(t_fdf *);

// Points in the map array (read from map file)
// Their x,y values are inferred from their positions in the array
typedef struct s_map_point
{
	short	z;
	int		color;
}	t_map_point;

// Simple 2D point structure for passing around points in graph functions
typedef struct s_point
{
	int	x;
	int	y;
	int	z;
	int	color;
}	t_point;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

// Used in Bresenham line algorithm
// Only here to save lines, to be below 26
// Not used in god struct, but needs t_point
typedef struct s_bresenham
{
	t_point	delta;
	int		step;
	int		error_term;
	t_color	c_i;
	t_color	c_step;
}	t_bresenham;

// This chained list contains file line by line (only used in map building)
typedef struct s_file_contents
{
	char					*line;
	struct s_file_contents	*next;
}	t_file_contents;

// Mlx img object
// - bypp	bytes per pixel (bpp / 8), in case compiler does not optimize
//			the redundant / 8 operation made on each pixel_put
typedef struct s_img
{
	void		*ptr;
	char		*addr;
	int			bpp;
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
	float	angle_x;
	float	angle_y;
	float	angle_z;
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
// - repeat			all data about key repeat
// - num_presses	how many keys were pressed last
// - states			state of key (off, on, repeat)
// - codes			array of physical key codes
// - actions		corresponding array of actions
// Used for keyboard interactions
typedef struct s_key
{
	t_repeat		repeat;
	int				num_presses;
	t_key_state		states[KEY_COUNT];
	int				codes[KEY_COUNT];
	t_key_action	actions[KEY_COUNT];
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
// - img_need_redraw	set to true on all interaction from user
typedef struct s_time
{
	struct timeval	current;
	struct timeval	last_refresh;
	struct timeval	last_fps;
	struct timeval	last_key_repeat;
	unsigned int	frame_count;
	unsigned int	loop_count;
	bool			img_need_redraw;
}	t_time;

// God struct
// - mlx	All minilibx data
// - file	File data used in parsing/building map
// - map	Map data (what we draw)
// - state	Engine states (how he draw it)
// - key	Key data (for key handlers)
// - mouse	Mouse data (for mouse/pointer handlers)
// - time	Time values (for syncing and time-sensitive calculations)
typedef struct s_fdf
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
void			init_key(t_key *key);
void			init_mouse(t_mouse *mouse);
bool			init_time(t_time *time);
// init_win_pos.c
void			init_zoom(t_fdf *d);
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
void			get_widths(t_fdf *d);

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
int				mouse_up_hook(int button, int x, int y, t_mouse *mouse);
// hook_keyboard.c
int				key_down_hook(int keycode, t_fdf *d);
int				key_up_hook(int keycode, t_key *key);
// hook_window_button.c
int				clientmsg_hook(t_fdf *d);

// INTERACTIVE
// interact_translate.c
void			move_right(t_fdf *d);
void			move_left(t_fdf *d);
void			move_up(t_fdf *d);
void			move_down(t_fdf *d);
void			move_x_mouse(t_fdf *d, int x);
void			move_y_mouse(t_fdf *d, int y);
// interact_zoom.c
void			zoom_in(t_fdf *d);
void			zoom_out(t_fdf *d);
void			zoom_in_mouse(t_fdf *d);
void			zoom_out_mouse(t_fdf *d);
// interact_rotate.c
void			rotate_x_increase(t_fdf *d);
void			rotate_x_decrease(t_fdf *d);
void			rotate_y_increase(t_fdf *d);
void			rotate_y_decrease(t_fdf *d);
void			rotate_z_increase(t_fdf *d);
void			rotate_z_decrease(t_fdf *d);
void			rotate_x_mouse(t_fdf *d, int x);
void			rotate_y_mouse(t_fdf *d, int y);
void			rotate_z_mouse(t_fdf *d, int z);
// interact_height.c
void			shift_up(t_fdf *d);
void			shift_down(t_fdf *d);
// interact_view.c
void			reset_view(t_fdf *d);
void			ortho_view_1(t_fdf *d);
void			ortho_view_2(t_fdf *d);
void			ortho_view_3(t_fdf *d);

//	RENDERING
// draw.c
void			reset_image(t_img *img);
void			draw_image(t_fdf *d);
// draw_3d.c
void			set_point(t_fdf *d, int x, int y, t_point *point);
void			set_point_init(t_fdf *d, int x, int y, t_point *point);
// draw_line.c
void			draw_line(t_img *img, t_point p1, t_point p2);
// draw_color.c
t_color			decompose_color(int color_int);
int				recompose_color(t_color color_rgb);
void			add_colors(t_color *c1, t_color c2);
void			get_color_step(t_color c1, t_color c2, int step,
					t_color *color_step);

#endif
