/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 13:45:00 by labintei          #+#    #+#             */
/*   Updated: 2021/09/13 18:04:59 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// redirection (NULL)
// ft_redirection -> cmds
//  > a > b

void		close_fd(t_list_file	**file)
{
	t_list_file		*temp;

	restart_t_list_file(file);
	temp = (*file);
	while(temp)
	{
		if(temp->redir != 'L')
			close(temp->fd);
		// DEUX FOIS LA MEME CHOSE ??
		if(temp->redir == 'L')
		{
			close(temp->pipe_fd[0]);
			close(temp->pipe_fd[1]);
		}
		temp = temp->next;
	}
}


int		close_pipes(t_list		*cmd, int is_piped)
{

	close_fd(&(cmd->file));
	if(cmd->previous && cmd->previous->type == '|')
		close(cmd->previous->pipe[0]);
	if(!is_piped)
		return(1);
	close(cmd->pipe[1]);
	if(!cmd->next)
		close(cmd->pipe[0]);
	return(1);
}

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

int			wait_exec_cmds(t_list		*cmds)
{
	int			ret;
	int			status;

	ret = 0;
	while(cmds)
	{
		if(cmds->pid != 0)
		{
			waitpid(cmds->pid, &status, 0);
			if(WIFEXITED(status))
				ret = WEXITSTATUS(status);
		}
		cmds = cmds->next;
	}
	return(ret);
}

// Close l entree et la sortie
//
// Si LA commande est pipe est piped il close(pipe[0])
// 
// Il dup les pipes et apres seulemnt il fait les dup_fd
//
//
int			exec_cmd(t_list *cmd, t_env *env)
{
	pid_t		pid;
	int			ret;
	int			is_piped;

	is_piped = 0;
	ret = 1;
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
		if(pipe(cmd->pipe))
			printf("\nErreur\n");
		is_piped = 1;
	}
	if(cmd->file)
		ft_redirection(env, cmd);
	pid = fork();
	cmd->pid = pid;
//	if(cmd->pid == -1)
//		printf("\nErreur\n");
	if(pid == 0 && cmd->pid == 0)
	{
		// DUP PIPES
		if(cmd->type == '|' && dup2(cmd->pipe[1], 1) < 0)
			printf("\nErreur\n");
		if(cmd->previous && cmd->previous->type == '|' && dup2(cmd->previous->pipe[0], 0) < 0)
			printf("\nErreur\n");
		if(cmd->file)
		// END
			ft_dup_fd2(cmd->file);
		if(cmd->cmds)
		{
			if(is_builtin(cmd->cmds[0]))
				ret = exec_build(cmd, env);
			else
				ret = exec_other(cmd, env);
		}
	}
	close_pipes(cmd, is_piped);
	return(ret);
}

int			exec_cmds(t_env *env)
{
	t_list		*c;
	int			ret;

	ret = 0;
	list_cmds_restart(&(env->cmds));
	c = env->cmds;
	while(c)
	{
		ret = exec_cmd(c, env);
		env->last_ret = ret;
		c = c->next;
	}
	wait_exec_cmds(env->cmds);
	return(ret);

}
