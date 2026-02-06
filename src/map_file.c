/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 15:19:21 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 16:22:02 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Called at the end of map-building + at exit
void	free_file(t_file_contents **file)
{
	t_file_contents	*last;
	t_file_contents	*cur;

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

static t_file_contents	*new_line(char *line)
{
	t_file_contents	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->line = line;
	new->next = NULL;
	return (new);
}

// Write file into a chained list with gnl
// Return # of lines
unsigned short	read_file(t_fdf *d)
{
	t_file_contents	*line_node;
	t_file_contents	*last_node;
	char			*line;
	unsigned short	i;

	i = 0;
	while (1)
	{
		if (i == (unsigned short)-1)
			error_out(d, ERRHEI);
		line = get_next_line(d->file.fd);
		if (errno)
			error_out(d, ERRREA);
		if (!line)
			return (i);
		line_node = new_line(line);
		if (!line_node)
			error_out(d, ERRMAL);
		if (!d->file.contents)
			d->file.contents = line_node;
		else
			last_node->next = line_node;
		last_node = line_node;
		i++;
	}
}
