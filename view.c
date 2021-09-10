/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 17:44:11 by labintei          #+#    #+#             */
/*   Updated: 2021/09/10 15:43:26 by mahautlatin      ###   ########.fr       */
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
	printf("LIST FILE :\n");
	while(temp)
	{
		if(temp && temp->redir)
			printf("TYPE: [%c]\n", temp->redir);
		if(temp && temp->path)
		{
			printf("PATH: [%s]\n", temp->path);
		}
		if (&temp->pipe_fd[0] && &temp->pipe_fd[1])
		{
			printf("pipe_fd[0] = [%i]\npipe_fd[1] = [%i]\n", temp->pipe_fd[0], temp->pipe_fd[1]);
		}
		if (temp->fd)
		{
			printf("fd: [%i]\n", temp->fd);
		}
		temp = temp->next;
	}
}

/* Pour savoir s'il y a une redirection il faut voir un  t_list_file */
void		view_cmds(t_list	**cmds)
{
	t_list	*read;

	list_cmds_restart(cmds);
	read = (*cmds);
	while(read)
	{
		if(read->cmds)
		{
			printf("VIEW CMDS\n");
			view_tab(read->cmds);
	//		printf("cmd->type: [%c]\n", read->type);
			if (&read->pipe[0] && &read->pipe[1])
				printf("cmd->pipe: [0: %i][1: %i]\n", read->pipe[0], read->pipe[1]);
		}
		if(read->file)
		{
			printf("VIEW T_LIST_FILE\n");
			view_t_list_file(&(read->file));
		}
		read = read->next;
	}
}
