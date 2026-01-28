/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:48:13 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 11:55:18 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Get the combined width of every line (for mallocing 1D map)
unsigned int	get_total_width(unsigned short *map_widths,
		unsigned short map_height)
{
	int				i;
	unsigned int	sum;

	sum = 0;
	i = 0;
	while (i < d->map_height)
	{
		sum += d->map_widths[i];
		i++;
	}
	return (sum);
}

// Returns 1 if the height value does not fit within a short
int	fill_point(t_point *point, char *line, int *i)
{
	int	ret;

	ret = ft_atox(line + *i, 0, &point->height, // We just check overflow
			sizeof(point->height) | ATOX_LAX); // Can use lax option because we already checked formatting
	if (ret < 0)
		return (1);
//	ft_printf("Just filled point with val %d\n", point->height);
	*i += ret;
	if (line[*i] == ',')
		*i += 1 + ft_atox(line + *i + 1, BASE16, &point->color, // ignoring return value, HAS to be positive
				sizeof(point->color) | ATOX_LAX); // Same, we already checked for 0xhhhhhh, even max value of this cannot overflow
	else
		point->color = -1;
//	ft_printf("Just filled color with val %d\n", point->color);
	return (0);
}

int	fill_line(t_point *map, char *line)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
		{
			if (fill_point(map + x, line, &i))
					return (1);
			x++;
		}
		else
			i++;
	}
	return (0);
}

// This has to not only fill the map but also write the correct addresses into the fake 2D map
void	fill_map(t_fdf d)
{
	int		line;
	int		i;

	line = 0;
	while (d.file)
	{
		d.map[line] = d.map_dat;
		i = 0;
		while (i < d.map_widths[i])
		{
			if (fill_line(d.map_dat, d.file->line))
				error_out(d, ERRVAL); //overflow
			i++;
		}
		d.map_dat += d.map_widths[i];
		line++;
		d.file = d.file->next;
	}
}

// 1. Try to open map (error: can't open)
// 2. Read map fully (error: can't read/too many lines)
// 3. Malloc address map and widths arrays (error: malloc)
// 4. Check lines, write all their widths (error: bad line format/line too long)
// 5. Malloc points map (error: malloc)
// 6. Fill map (error: overflow)
// 7. Free fd/file
void	get_map(t_fdf *d)
{
	unsigned int	total_width;

	d->fd = open(d->av[1], O_RDONLY);
	if (d->fd == -1)
		error_out(*d, ERROPN);
	d->map_height = read_file(d); //rd file -> newline
	if (d->map_height)
	{
		d->map = malloc(sizeof(*d->map) * d->map_height);
		d->map_widths = malloc(sizeof(*d->map_widths) * d->map_height);
		if (!d->map || !d->map_widths)
			error_out(*d, ERRMAL);
		get_widths(*d); //get widths -> get width -> is point valid
		total_width = get_total_width(d->map_widths, d->map_height); //get tot width
		if (total_width)
		{
			d->map = malloc(sizeof(*d->map) * total_width);
			if (!d->map)
				error_out(*d, ERRMAL);
			fill_map(d); //fill map -> fill line -> fill point
		}
	}
	free_file(&d->file); //free file (exit)
	ft_close(&d->fd);
}
