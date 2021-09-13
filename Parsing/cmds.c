/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:26:15 by labintei          #+#    #+#             */
/*   Updated: 2021/09/13 21:56:08 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		add_cmds(t_list		**cmds)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if(!new)
		return ;
	new->type = '0';
	new->next = NULL;
	new->cmds = NULL;
	new->pipe[0] = 0;
	new->pipe[1] = 1;
	new->is_piped = 0;
	if((*cmds))
	{
		(*cmds)->next = new;
//		new->pipe[0] = 0;
//		new->pipe[1] = 1;
		new->previous = (*cmds);
		(*cmds) = (*cmds)->next;
	}
	else
	{
		new->previous = NULL;
		(*cmds) = new;
	}
}
