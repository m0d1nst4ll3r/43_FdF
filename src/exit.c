/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:20:08 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/07 14:38:46 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	destroy_mlx(t_mlx *mlx)
{
	if (mlx->img.ptr)
		mlx_destroy_image(mlx->ptr, mlx->img.ptr);
	if (mlx->win)
		mlx_destroy_window(mlx->ptr, mlx->win);
	if (mlx->ptr)
	{
		mlx_do_key_autorepeaton(mlx->ptr);
		mlx_destroy_display(mlx->ptr);
		free(mlx->ptr);
	}
}

static void	destroy_map(t_map *map)
{
	if (map->data)
		free(map->data);
	if (map->index)
		free(map->index);
	if (map->widths)
		free(map->widths);
}

static void	destroy_file(t_file *file)
{
	if (file->contents)
		free_file(&file->contents);
	ft_close(&file->fd);
}

void	exit_prog(t_fdf *d, unsigned char exitval)
{
	destroy_file(&d->file);
	destroy_map(&d->map);
	destroy_mlx(&d->mlx);
	exit(exitval);
}
