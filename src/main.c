/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/05 22:00:34 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

//	Program steps
// 1. Init all values
//		- do not init mlx yet, this would open a window
// 2. Build map
//		- this might fail, in which case we error out
// 3. Init mlx
//		- this displays window
// 4. Set up all hooks
//		- keyboard
//		- mouse
//		- game loop
// 5. Enter mlx loop
//		- mandatory draw on first game loop
//		- now user can interact with program
//		- exit on ESC or X window button
//	6. Whether we exit normally or through an error, always free all
int	main(int ac, char **av)
{
	t_fdf	data;

	if (ac != 2)
		return (0);
	init_prog(&data, av[1]);
	get_map(&data);
	init_mlx(&data);
	set_hooks(&data);
	mlx_loop(data.mlx);
	exit_prog(&data, 0);
}
