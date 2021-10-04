/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:01:17 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 11:02:23 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	close_pipes(t_list	*cmd, int is_piped)
{
	close_fd(&(cmd->file));
	if (cmd->previous && cmd->previous->type == '|')
		close(cmd->previous->pipe[0]);
	if (!is_piped)
		return (1);
	close(cmd->pipe[1]);
	if (!cmd->next)
		close(cmd->pipe[0]);
	return (1);
}

int	error_exec_redir(t_list_file *cmds)
{
	t_list_file		*temp;

	temp = cmds;
	while (temp)
	{
		if (temp && temp->error)
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	ft_dup_fd2(t_list_file *cmd)
{
	t_list_file		*temp;

	temp = cmd;
	while (temp)
	{
		if (temp->redir == '>' || temp->redir == 'R')
			dup2(temp->fd, 1);
		else
		{
			if (temp->redir == '<')
				dup2(temp->fd, 0);
			else
			{
				dup2(temp->pipe_fd[0], 0);
				close(temp->pipe_fd[0]);
			}
		}
		temp = temp->next;
	}
}

void	exec_cmd(t_list *cmd, t_env *env)
{
	cmd->pipe_open = 0;
	if (cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
		cmd->pipe_open = 1;
		if (pipe(cmd->pipe))
			return ;
	}
	if (cmd->file)
	{
		restart_t_list_file(&(cmd->file));
		if (cmd->error == 0 && ft_redirection(cmd->file, env, 0) == -1)
		{
			cmd->error = 3;
		}
	}
	cmd->pid = fork();
	inhibit_signals(cmd->pid);
	if (cmd->pid == 0)
		exec_pipe(cmd, env);
	close_pipes(cmd, cmd->pipe_open);
}

int	wait_heredoc(t_list_file	**file)
{
	t_list_file		*f;

	restart_t_list_file(file);
	f = (*file);
	while (f)
	{
		if (f && f->redir == 'L')
			return (1);
		f = f->next;
	}
	return (0);
}
