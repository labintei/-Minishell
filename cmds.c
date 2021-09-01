/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:26:15 by labintei          #+#    #+#             */
/*   Updated: 2021/08/27 13:16:10 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		add_cmds(t_list		**cmds)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if(!new)
		return ;
	new->type = '0';
	new->next = NULL;
	new->cmds = NULL;
	if((*cmds))
	{
		(*cmds)->next = new;
		new->previous = (*cmds);
		(*cmds) = (*cmds)->next;
	}
	else
	{
		new->previous = NULL;
		(*cmds) = new;
	}
}
