/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 11:30:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	display_map_to_make_sure_i_didnt_fuck_it_up(t_fdf data)
{
	int	x;
	int	y;

	y = 0;
	ft_printf("Map has %d lines\nLine lengths:\n", data.map_height);
	while (y < data.map_height)
	{
		ft_printf("Line %2d is %3d points long\n", y, data.map_widths[y]);
		y++;
	}
	y = 0;
	while (y < data.map_height)
	{
		x = 0;
		while (x < data.map_widths[y])
		{
			ft_printf("% 5hd,%#8X  ", data.map[y][x].height, data.map[y][x].color);
			x++;
		}
		ft_printf("\n");
		y++;
	}
}

//	Steps:
// 0. Notes
//		- This is WIP and some artifacts from earlier testing were left behind
//		- For now hooks are WIP, lmb_held/rmb_held, frame related vars
//			and time related vars are temporary/WIP (so ignore them)
// 1. Init prog
//		a. Init all struct address values to NULL in case of exit, set av
//		(we do not init MLX because program might fail at map-building)
//		(init MLX = window pops up = not a good look if we can't even read map)
// 2. Read file and build map
//		a. Try to open it, try to read it, infer map_height
//		(read = fill a chained list, each node is a line, only parse file once)
//		(an empty map is valid)
//		b. Check each line for its valid format and width, write map_widths
//		(lines may have different lengths or be empty)
//		c. Malloc map (we had to wait since it has to be 1 big block)
//		d. Fill map (this might still fail, this is where we check for overflow)
// 3. ... ??? Stuff
// 		- At this point file is read and map is built
//
int	main(int ac, char **av)
{
	t_fdf	data;

	if (ac != 2)
		return (0);
	init_prog(&data, av); // Init MLX, av, and malloc addresses - can fail
	get_map(&data); // Read map and store it somewhere - can fail
	display_map_to_make_sure_i_didnt_fuck_it_up(data);
	//init_mlx(&data); // init mlx - displays window! we do it after map checking etc..
	//set_hooks(&data); // cannot fail
	//mlx_loop(data.mlx); // Loop
	exit_prog(data, 0);
}
