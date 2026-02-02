/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/02 17:51:02 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
// 3. Interpret map to draw our first picture
//		- Map is interpreted in a very simplistic way, no colors, bad movement
//		- WIP on HOOKS and INTERACTIVITY
//		- Next step is to improve VISUALIZATION and MOVEMENT
//		- Next step is to code COLORS in
//		- Next step is to code DEPTH vision blocking
//		- Possible next steps: fixing useless out-of-screen calculations
//		- Default colors - color palettes
//		- Background color
//
int	main(int ac, char **av)
{
	t_fdf	data;

	if (ac != 2)
		return (0);
	init_prog(&data, av);
	get_map(&data);
	init_mlx(&data);
	set_hooks(&data);
	mlx_loop(data.mlx);
	exit_prog(data, 0);
}
