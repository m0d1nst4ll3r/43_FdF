/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 15:19:21 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/17 16:51:01 by rapohlen         ###   ########.fr       */
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
		free(cur->line);
		free(cur);
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
			error_out(d, ERRREA);
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

int	is_point_valid(char *line, int *i)
{
	int	count;

	while (ft_isdigit(line[*i]))
		*i++;
	if (line[*i] == ',')
	{
		count = 0;
		*i++;
		while (count < 6 && ft_ishexa(line[*i]))
		{
			i++;
			count++;
		}
		if (!count)
			return (0);
	}
	if (line[*i] && !ft_isspace(line[*i]))
		return (1);
}

int	get_line_size_if_valid(char *line)
{
	int	i;
	int	j;
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

// Returns map size, but also checks for map validity
int	get_map_size_if_valid(t_file *file)
{
	int	line_size;
	int	line;
	int	col;

	line = 0;
	col = 0;
	while (file)
	{
		line++;
		line_size = get_line_size_if_valid(file->line);
		if (!line_size)
			return (0);
		if (!col)
			col = line_size;
		else if (col != line_size)
			return (0);
		file = file->next;
	}
	return (line * col);
}

// This reads the file. In order, it:
// 1. Tries to open the file, error if it can't
// 2. Reads the file progressively, adding every line to a list, error if read ever fails
// 3. Processes each line, verifying map validity, error if map is invalid
// 4. Finally, mallocs the map data array and fills it
void	get_map(t_fdf *d)
{
	t_file	*file;
	int		fd;
	int		map_size;

	fd = open(d->av[1], O_RDONLY);
	if (fd == -1)
		error_out(*d, ERROPN);
	read_file(d, fd);
	if (!d->file)
		error_out(*d, ERREMP);
	map_size = get_map_size_if_valid(d->file);
	if (!map_size)
		error_out(*d, ERRMAP);
	fill_map(d);
	free_file(&d->file);
}
