/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 17:44:11 by labintei          #+#    #+#             */
/*   Updated: 2021/08/30 18:22:57 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		list_cmds_restart(t_list	**cmds)
{
	while((*cmds) && (*cmds)->previous)
		(*cmds) = (*cmds)->previous;
}

void		view_cmds(t_list	**cmds)
{
	int			i;
	t_list	*read;

	i = 0;
	list_cmds_restart(cmds);
	read = (*cmds);
	while(read)
	{
		printf("\nCMDS %d\n", i);
		i++;
		if(read->cmds && read->cmds[0])
			view_tab(read->cmds);
		(read) = read->next;
	}
}
