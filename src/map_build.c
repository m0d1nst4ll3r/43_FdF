/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:48:13 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 20:00:09 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Get the combined width of every line (for mallocing 1D map)
static unsigned int	get_total_width(const t_map *map)
{
	int				i;
	unsigned int	sum;

	sum = 0;
	i = 0;
	while (i < map->height)
	{
		sum += map->widths[i];
		i++;
	}
	return (sum);
}

// Returns 1 if the height value does not fit within a short
static int	fill_point(t_map_point *point, char *line, int *i)
{
	int	ret;

	ret = ft_atox(line + *i, 0, &point->z,
			sizeof(point->z) | ATOX_LAX);
	if (ret < 0)
		return (1);
	*i += ret;
	if (line[*i] == ',')
		*i += 1 + ft_atox(line + *i + 1, BASE16, &point->color,
				sizeof(point->color) | ATOX_LAX);
	else
		point->color = DEFAULT_COLOR;
	return (0);
}

static int	fill_line(t_map_point *map, char *line)
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

// Fill map_data and also index it into map array
// Also check for overflowing values
static void	fill_map(t_fdf *d)
{
	t_map_point		*map_tmp;
	t_file_contents	cur;
	int				line;

	map_tmp = d->map.data;
	cur = d->file.contents;
	line = 0;
	while (cur)
	{
		d->map.index[line] = map_tmp;
		if (fill_line(map_tmp, cur->line))
			error_out(d, ERRVAL);
		map_tmp += d->map.widths[line];
		line++;
		cur = cur->next;
	}
}

//	Map building steps
// 1. Try to open map (error: can't open)
// 2. Read map fully (error: can't read/too many lines/malloc)
// 3. Malloc address map and widths arrays (error: malloc)
// 4. Check lines, write all their widths (error: bad line format/line too long)
// 5. Malloc points map (error: malloc)
// 6. Fill map (error: overflow)
// 7. Free fd/file
void	get_map(t_fdf *d)
{
	unsigned int	total_width;

	d->file.fd = open(d->file.name, O_RDONLY);
	if (d->file.fd == -1)
		error_out(d, ERROPN);
	d->map.height = read_file(d);
	if (d->map.height)
	{
		d->map.data = malloc(sizeof(*d->map.data) * d->map.height);
		d->map.widths = malloc(sizeof(*d->map.widths) * d->map.height);
		if (!d->map.data || !d->map.widths)
			error_out(d, ERRMAL);
		get_widths(d);
		total_width = get_total_width(&d->map);
		if (total_width)
		{
			d->map.index = malloc(sizeof(*d->map.index) * total_width);
			if (!d->map.index)
				error_out(d, ERRMAL);
			fill_map(d);
		}
	}
	free_file(&d->file.contents);
	ft_close(&d->file.fd);
}
