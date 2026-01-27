/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 15:19:21 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/27 18:47:51 by rapohlen         ###   ########.fr       */
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

unsigned short	read_file(t_fdf *d, int fd)
{
	t_file			*line_node;
	t_file			*last_node;
	char			*line;
	unsigned short	i;

	i = 0;
	while (1)
	{
		if (i == (unsigned short)-1)
			error_out(*d, ERRHEI);
		line = get_next_line(fd);
		if (errno)
			error_out(*d, ERRREA);
		if (!line)
			return (i);
		line_node = new_line(line);
		if (!line_node)
			error_out(*d, ERRMAL);
		if (!d->file)
			d->file = line_node;
		else
			last_node->next = line_node;
		last_node = line_node;
		i++;
	}
}

// Returns 1 if the height value does not fit within a short
int	fill_point(t_point *point, char *line, int *i)
{
	int	ret;

	ret = ft_atox(line + *i, 0, &point->height,
			sizeof(point->height) | ATOX_LAX); //TODO: STOPPED HERE
	if (ret < 0)
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
				error_out(d, ERRVAL);
			i++;
		}
		d.map_dat += d.map_widths[i];
		line++;
		d.file = d.file->next;
	}
}

// A valid point is -?[0-9]+(,0x[0-9a-fA-F]{1,6}])?
// One or more digits followed by an optional group of comma, a 0x,
//	and between 1 and 6 hexadecimal characters
// We are not yet checking if the values fit within their type
int	is_point_valid(char *line, int *i)
{
	int	count;

	if (line[*i] == '-')
		(*i)++;
	if (!ft_isdigit(line[*i]))
		return (0);
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

// Returns line size or -1 if not valid
int	get_width(t_fdf d, char *line)
{
	int	i;
	int	width;

	i = 0;
	width = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
		{
			if (width == (unsigned short)-1)
				error_out(d, ERRWID);
			width++;
			if (!is_point_valid(line, &i))
				error_out(d, ERRMAP);
		}
		else
			i++;
	}
	return (width);
}

// Parse file to write widths in width array
void	get_widths(t_fdf d)
{
	int	i;

	i = 0;
	while (d.file)
	{
		d.map_widths[i] = get_width(d, d.file->line);
		if (d.map_widths[i] == -1)
			error_out(d, ERRMAP);
		d.file = d.file->next;
		i++;
	}
}

// Get the combined width of every line (for mallocing 1D map)
unsigned int	get_total_map_width(unsigned short *map_widths,
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

// This reads the file. In order, it:
// 1. Tries to open the file, error if it can't
// 2. Reads the file progressively, adding every line to a list,
//		error if read ever fails or if file is empty
// 3. Processes each line, verifying map validity, error if map is invalid
//		Map is invalid if:
//		a. Inconsistent line sizes
//		b. 
// 4. Finally, mallocs the map data array and fills it
// Changing all of this for flex map
// 1. Should still try to open, error if it can't
// 2. Should read the file, error if it fails
// 1 and 2 don't change
//		a. Now we should try to malloc map_widths with map_height (# of lines we just wrote)
//		b. We should also malloc map with map_height (will contain addresses)
// 3. When we process lines, we now allow different line lengths, and write the length into map_widths
//		(we do not actually malloc the real map or write anything, we need to know total line lengths)
// 4. Once this is done and there has been no error, we malloc map_dat of the sum of all line lengths we got
// 		a. We also now write the addresses into map, and we write the actual points
// 	Once all this is done, our map is finally fucking written (fucking phew)
void	get_map(t_fdf *d)
{
	int				fd;
	unsigned int	total_width;

	fd = open(d->av[1], O_RDONLY); // 1.
	if (fd == -1)
		error_out(*d, ERROPN);
	d->map_height = read_file(d, fd); // 2.
//	if (!d->map_height)
//		error_out(*d, ERREMP);
	if (d->map_height) // We are now allowing completely empty maps. Fuck it!
	{
		d->map = malloc(sizeof(*d->map) * d->map_height); // a.
		d->map_widths = malloc(sizeof(*d->map_widths) * d->map_height); // b.
		if (!d->map || !d->map_widths)
			error_out(*d, ERRMAL);
		get_widths(*d); // 3.
		total_width = get_total_map_width(d->map_widths, d->map_height);
		if (total_width)
		{
			d->map = malloc(sizeof(*d->map) * total_width);
			if (!d->map) // 4.
				error_out(*d, ERRMAL);
			fill_map(d); // a. (can fail due to overflow)
		}
	}
	// Remember, at this point map can be NULL if there were no lines or they were all empty
	// map_height doesn't have to be 0 though, and map_widths can be an array with 0's inside
	free_file(&d->file); // Map is written, don't need file anymore
}
