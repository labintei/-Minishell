/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:39:47 by labintei          #+#    #+#             */
/*   Updated: 2021/10/03 23:51:02 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_a_n(char *s)
{
	int		i;

	i = 0;
	if (!s && !(*s))
		return (0);
	if (s && s[i] == '-')
		i++;
	else
		return (0);
	while (s && s[i] == 'n')
		i++;
	if (!s[i])
		return (1);
	else
		return (0);
}

int	echo_build(t_list *cmds)
{
	int		i;
	int		g;

	i = 1;
	g = 0;
	while (cmds->cmds && cmds->cmds[0] && cmds->cmds[i] && \
	is_a_n(cmds->cmds[i]))
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
