/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:20:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 15:41:11 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	exit_prog(t_fdf d, unsigned char exitval)
{
	if (d.map)
		free(d.map);
	if (d.map_dat)
		free(d.map_dat);
	if (d.map_widths)
		free(d.map_widths);
	if (d.file)
		free_file(&d.file);
	ft_close(&d.fd);
	if (d.img.img)
		mlx_destroy_image(d.mlx, d.img.img);
	if (d.win)
		mlx_destroy_window(d.mlx, d.win);
	if (d.mlx)
	{
		mlx_destroy_display(d.mlx);
		free(d.mlx);
	}
	exit(exitval);
}

void	error_out(t_fdf d, char *s)
{
	ft_fprintf(2, "fdf: %s", s);
	if (errno)
		ft_fprintf(2, ": %s", strerror(errno));
	ft_fprintf(2, "\n");
	exit_prog(d, 1);
}
