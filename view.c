/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 17:44:11 by labintei          #+#    #+#             */
/*   Updated: 2021/09/08 16:04:53 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		list_cmds_restart(t_list	**cmds)
{
	while((*cmds) && (*cmds)->previous)
		(*cmds) = (*cmds)->previous;
}
/*
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
}*/

void		view_t_list_file(t_list_file	**read)
{
	t_list_file		*temp;
	
	restart_t_list_file(read);
	temp = *read;
	printf("\nLIST FILE PATH :\n");
	while(temp)
	{
		if(temp && temp->redir)
			printf("\n De type %c\n", temp->redir);
		if(temp && temp->path)
		{
			printf("\nPATH : %s\n", temp->path);
		}
		temp = temp->next;
	}
}




void		view_cmds(t_list	**cmds)
{
	t_list	*read;
	
	list_cmds_restart(cmds);
	read = (*cmds);
	while(read)
	{
		if(read->cmds)
		{
			printf("\nVIEW CMDS\n");
			view_tab(read->cmds);
		}
		if(read->file)
		{
			printf("\nVIEW T_LIST_FILE\n");
			view_t_list_file(&(read->file));
		}
		read = read->next;
	}
}
