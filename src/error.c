/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 01:18:05 by rapohlen          #+#    #+#             */
/*   Updated: 2026/02/06 01:19:53 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	error_out(t_fdf *d, char *err_str)
{
	if (!err_str)
		err_str = DEFAULT_ERR;
	ft_fprintf(2, "fdf: %s", err_str);
	if (errno)
		ft_fprintf(2, ": %s", strerror(errno));
	ft_fprintf(2, "\n");
	exit_prog(d, 1);
}
