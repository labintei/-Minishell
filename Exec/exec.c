/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:41:12 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 18:06:09 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_dup_fd2(t_list_file *cmd)
{
	t_list_file		*temp;

	temp = cmd;
	while(temp)
	{
		if(temp->redir == '>' || temp->redir == 'R')
			dup2(temp->fd, 1);
		else
		{
			if(temp->redir == '<')
			{
				dup2(temp->fd, 0);
			}
			else
			{
				dup2(temp->pipe_fd[0], 0);
				close(temp->pipe_fd[0]);
			}
		}
		temp = temp->next;
	}
}
