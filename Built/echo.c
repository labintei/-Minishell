/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:39:47 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:57:27 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	echo_build(t_list *cmds)
{
	int		i;
	int		g;

	i = 1;
	g = 0;
	if (cmds->cmds && cmds->cmds[0] && cmds->cmds[1] && \
	ft_strcmp(cmds->cmds[i], "-n"))
	{
		g = 1;
		i++;
	}
	while (cmds->cmds && cmds->cmds[0] && cmds->cmds[i])
	{
		write(1, cmds->cmds[i], ft_strlen(cmds->cmds[i]));
		if (cmds->cmds[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (g != 1)
		write(1, "\n", 1);
	return (0);
}
