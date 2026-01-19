/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 15:19:21 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/19 16:33:42 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_file(t_file **file)
{
	t_file	*last;
	t_file	*cur;

	cur = *file;
	while (cur)
	{
		last = cur;
		cur = cur->next;
		free(last->line);
		free(last);
	}
	*file = NULL;
}

t_file	*new_line(char *line)
{
	t_file	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->line = line;
	new->next = NULL;
	return (new);
}

void	read_file(t_fdf *d, int fd)
{
	t_file	*line_node;
	t_file	*last_node;
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (errno)
			error_out(*d, ERRREA);
		if (!line)
			return ;
		line_node = new_line(line);
		if (!line_node)
			error_out(*d, ERRMAL);
		if (!d->file)
			d->file = line_node;
		else
			last_node->next = line_node;
		last_node = line_node;
	}
}

// A valid point is [0-9]+(,0x[0-9a-fA-F]{1,6}])?
// One or more digits followed by an optional group of comma, a 0x,
//	and between 1 and 6 hexadecimal characters
// We are not yet checking if the digits fit within a short
int	is_point_valid(char *line, int *i)
{
	int	count;

	if (line[*i] != '-' && !ft_isdigit(line[*i]))
		return (0);
	if (line[*i] == '-')
		(*i)++;
	while (ft_isdigit(line[*i]))
		(*i)++;
	if (line[*i] == ',' && line[*i + 1] == '0' && line[*i + 2] == 'x')
	{
		count = 0;
		*i += 3;
		while (count < 6 && ft_ishexa(line[*i]))
		{
			(*i)++;
			count++;
		}
		if (!count)
			return (0);
	}
	if (line[*i] && !ft_isspace(line[*i]))
		return (0);
	return (1);
}

// Returns line size or 0 if not valid
int	is_line_valid(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
		{
			count++;
			if (!is_point_valid(line, &i))
				return (0);
		}
		else
			i++;
	}
	return (count);
}

// Checks for map validity
// Also fills in map_width and map_height
int	is_map_valid(t_fdf *d)
{
	t_file	*cur;
	int		line_size;

	d->map_height = 0;
	cur = d->file;
	while (cur)
	{
		line_size = is_line_valid(cur->line);
		if (!line_size)
			return (0);
		if (!d->map_height && line_size)
			d->map_width = line_size;
		else if (!line_size || d->map_width != line_size)
		{
			ft_printf("Bad line_size! Expected: %d, got %d\n", d->map_width, line_size);
			return (0);
		}
		d->map_height++;
		cur = cur->next;
	}
	return (1);
}

// Returns 1 if the height value does not fit within a short
int	fill_point(t_point *point, char *line, int *i)
{
	if (ft_atos(line + *i, &point->height))
		return (1);
	ft_printf("Just filled point with val %d\n", point->height);
	while (ft_isdigit(line[*i]))
		(*i)++;
	if (line[*i] == ',')
	{
		*i += 3;
		point->color = ft_atoh(line + *i);
		while (ft_ishexa(line[*i]))
				(*i)++;
	}
	else
		point->color = -1;
	ft_printf("Just filled color with val %d\n", point->color);
	return (0);
}

int	fill_line(t_point *map, char *line)
{
	int		i;
	int		x;

	i = 0;
	x = 0;
	while (line[i])
	{
		ft_printf("Testing line[%d]: %c\n", i, line[i]);
		if (ft_isdigit(line[i]))
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

void	fill_map(t_fdf *d)
{
	t_file	*cur;
	int		y;

	d->map = malloc(sizeof(*d->map) * (d->map_width * d->map_height));
	if (!d->map)
		error_out(*d, ERRMAL);
	cur = d->file;
	y = 0;
	while (cur)
	{
		if (fill_line(d->map + y * d->map_width, cur->line))
			error_out(*d, ERRVAL);
		y++;
		cur = cur->next;
	}
}

// This reads the file. In order, it:
// 1. Tries to open the file, error if it can't
// 2. Reads the file progressively, adding every line to a list,
//		error if read ever fails or if file is empty
// 3. Processes each line, verifying map validity, error if map is invalid
//		Map is invalid if:
//		a. Inconsistent line sizes
//		b. 
// 4. Finally, mallocs the map data array and fills it
void	get_map(t_fdf *d)
{
	int		fd;

	fd = open(d->av[1], O_RDONLY);
	if (fd == -1)
		error_out(*d, ERROPN);
	read_file(d, fd);
	if (!d->file)
		error_out(*d, ERREMP);
	if (!is_map_valid(d))
		error_out(*d, ERRMAP);
	fill_map(d);
	free_file(&d->file);
}
