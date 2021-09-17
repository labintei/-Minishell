/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 13:45:00 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 16:10:59 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// redirection (NULL)
// ft_redirection -> cmds
//  > a > b

int		exec_other(t_list	*c, t_env *env)
{
	char	**test;
	int		ret;

	if(c->is_fork == 0)
	{
		c->is_fork = 1;
		c->pid = fork();
//		inhibit_signals(c->pid);
	}
	ret = 0;
	//inhibit_signals(c->pid);
	if(c->pid == 0)
	{
		if(c->cmds && c->cmds[0] && c->cmds[0][0] && c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
		{
			ft_convert_env(&(env->env), &test);
			ret = execve(c->cmds[0], c->cmds, test);
			if(test)
				clear_tab(&test);
			return(ret);
		}
		if(c->cmds && c->cmds[0])
		{
			find_exec_path(&(c->cmds[0]), env);
			ft_convert_env(&(env->env), &test);
			ret = execve(c->cmds[0], c->cmds, test);
			if(test)
				clear_tab(&test);
		}
	}
	return(ret);
}

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
		if(cmds && cmds->cmds && cmds->is_fork)
		{
			waitpid(cmds->pid, &status, 0);
			if(WIFEXITED(status))
				ret = WEXITSTATUS(status);
		}
		cmds = cmds->next;
	}
//	if(cmds->is_fork)
//	handle_signals();
	return(ret);
}

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
		cmd->is_piped = 1;
	}
	if(cmd->file)
		ft_redirection(env, cmd);
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
		pid = fork();
		cmd->pid = pid;
		cmd->is_fork = 1;
//		inhibit_signals(cmd->pid);
//		cmd->pid = pid;
//		cmd->is_fork = 1;
		if(pid == 0 && cmd->pid == 0)
		{
			if(cmd->type == '|' && dup2(cmd->pipe[1], 1) < 0)
				printf("\nErreur\n");
			if(cmd->previous && cmd->previous->type == '|' && dup2(cmd->previous->pipe[0], 0) < 0)
				printf("\nErreur\n");
			if(cmd->file)
				ft_dup_fd2(cmd->file);
			if(is_builtin(cmd->cmds[0]))
				exit(ret = exec_build(cmd, env));
			if(is_piped)
				close(cmd->pipe[0]);
			if(cmd->cmds /*&& !(is_builtin(cmd->cmds[0]))*/)
			{
				if(cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
					exit(ret = exec_build(cmd, env));
				else
					exit(ret = exec_other(cmd, env));
			}
		}
	}
	else
	{
		if(cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
			ret = exec_build(cmd, env);
		else
			ret = exec_other(cmd, env);
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
