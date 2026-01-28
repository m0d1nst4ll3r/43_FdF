/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_valid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 11:53:56 by rapohlen          #+#    #+#             */
/*   Updated: 2026/01/28 12:01:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Valid point regex: -?[0-9]+(,0x[0-9a-fA-F]{1,6}])?
// - Optional '-'
// - One or more digits
// - Optional group of
//		- ",0x"
//		- Between 1 and 6 hexadecimal characters
// Note that overflow is not being checked yet
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

// Returns line size
// Also checks for line format
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

// Writes line widths in map_widths array
// Also checks for line format
void	get_widths(t_fdf d)
{
	int	i;

	i = 0;
	while (d.file)
	{
		d.map_widths[i] = get_width(d, d.file->line);
		d.file = d.file->next;
		i++;
	}
}
