/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 11:58:06 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/16 16:45:02 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Quick & dirty .ber check that I'd forgotten
static bool	is_ber_file(char *filename)
{
	int	i;

	i = 0;
	while (filename[i])
		i++;
	if (i < 5
		|| filename[i - 1] != 'r'
		|| filename[i - 2] != 'e'
		|| filename[i - 3] != 'b'
		|| filename[i - 4] != '.')
		return (false);
	return (true);
}

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
//		- engine loop
// 5. Enter mlx loop
//		- mandatory draw on first engine loop
//		- now user can interact with program
//		- exit on ESC or X window button
//	6. Whether we exit normally or through an error, always free all
int	main(int ac, char **av)
{
	t_fdf	data;

	if (ac != 2)
		return (0);
	if (!is_ber_file(av[1]))
	{
		ft_printf("fdf: Map needs to be a .ber file\n");
		return (0);
	}
	init_prog(&data, av[1]);
	get_map(&data);
	init_zoom(&data);
	init_mlx(&data);
	set_hooks(&data);
	mlx_loop(data.mlx.ptr);
	exit_prog(&data, 0);
}
