/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/19 11:41:59 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// We don't know how much to malloc our map
// Until we open the file and start reading it
// We can either remalloc all the time
// Or we can read entirely once just to figure out how many lines and columns
// (and we might as well just verify the validity of the map entirely while we're at it)
//
// Which method?
// 1. Lots and lots of malloc and free calls and re-copying stuff. Bad.
// 2. Same tbh because reading file twice causes as many mallocs/frees as there are lines.
// 3. Think of another way?
// Put all the lines in a chained list, verify validity of all the lines, then malloc
// This is basically option 2 but without having to read the file twice and close/reopen

int	main(int ac, char **av)
{
	t_fdf	data;

	if (ac != 2)
		return (0);
	init_prog(&data, av); // Init MLX, av, and malloc addresses - can fail
	get_map(&data); // Read map and store it somewhere - can fail
	set_hooks(&data); // cannot fail
	// Draw first image and put to window
	mlx_loop(data.mlx); // Loop
	// exit_prog will be called by events (hitting Esc or clicking X)
}
