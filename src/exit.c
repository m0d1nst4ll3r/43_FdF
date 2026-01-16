/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:20:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/16 00:15:38 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	exit_prog(t_fdf d, unsigned char exitval)
{
	// also free any mallocs, but none for now
	if (d.img.img)
		mlx_destroy_image(d.mlx, d.img.img);
	if (d.win)
		mlx_destroy_window(d.mlx, d.win);
	if (d.mlx)
	{
		mlx_destroy_display(d.mlx);
		free(d.mlx); // have to free mlx for some reason because mlx_destroy_display doesn't do it
	}
	exit(exitval);
}

void	error_out(t_fdf d, char *s)
{
	ft_fprintf(2, "fdf: %s", s); // display error
	if (errno)
		ft_fprintf(2, ": %s", strerror(errno)); // if error was open, read etc, display errno string
	ft_fprintf(2, "\n");
	exit_prog(d, 1);
}
