/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 13:45:00 by labintei          #+#    #+#             */
/*   Updated: 2021/09/21 14:13:21 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// redirection (NULL)
// ft_redirection -> cmds
//  > a > b

void		close_fd(t_list_file	**file);


int		exec_other(t_list	*c, t_env *env)
{
	char	**test;
	int		ret;

	ret = 0;
	if(c->pid == 0)
	{
		if(c->cmds && c->cmds[0] && c->cmds[0][0] && c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
		{
			ft_convert_env(&(env->env), &test);
			ret = execve(c->cmds[0], c->cmds, test);
			if(test)
				clear_tab(&test);
			exit(ret);
		}
		else if(c->cmds && c->cmds[0])
		{
			find_exec_path(&(c->cmds[0]), env);
			ft_convert_env(&(env->env), &test);
			ret = execve(c->cmds[0], c->cmds, test);
			if(test)
				clear_tab(&test);
			exit(ret);
		}
		exit(ret);
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
	return(ret);
}

void		ft_find_redirection(t_list_file		**file)
{
	t_list_file		*redir;
	int				i;
	int				j;

	i = 0;
	j = 0;
	restart_t_list_file(file);
	redir = (*file);
	while(redir)
	{
		if(i == 0 && (redir->redir == 'L' || redir->redir == '<'))
		{
			 i = 1;
			close(0);
		}
		if(j == 0 && (redir->redir == 'R' || redir->redir == '>'))
		{
			j = 1;
			close(1);
		}
		redir = redir->next;
	}
}

int			exec_cmd(t_list *cmd, t_env *env)
{
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
	{
		restart_t_list_file(&(cmd->file));
		ft_redirection(cmd->file, env);
		restart_t_list_file(&(cmd->file));
	}
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
		exec_pipe(cmd, env, is_piped);
	else
	{
		if(cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
			exec_build_not_pipe(cmd, env);
		else
			exec_not_build_not_pipe(cmd, env);
	}
	close_pipes(cmd, is_piped);
	return(ret);
}

int			exec_pipe(t_list *cmd, t_env *env, int is_piped)
{
	pid_t		pid;
	int			ret;

	(void)is_piped;
	ret = 0;
	pid = fork();
	cmd->pid = pid;
	cmd->is_fork = 1;
	if(/*pid == 0 &&*/ cmd->pid == 0)
	{
		if(cmd->type == '|'  && dup2(cmd->pipe[1], 1) < 0)
			printf("\nErreur\n");
		if(cmd->previous && cmd->previous->type == '|'  && dup2(cmd->previous->pipe[0], 0) < 0)
			printf("\nErreur\n");
		if(cmd->file)
			ft_dup_fd2(cmd->file);
//		if(is_builtin(cmd->cmds[0]))
//			exit(ret = exec_build(cmd, env));
//		if(is_piped)
//			close(cmd->pipe[0]);
		if(cmd->cmds /*&& !(is_builtin(cmd->cmds[0]))*/)
		{
			if(cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
				exit(ret = exec_build(cmd, env));
			else
				exit(ret = exec_other(cmd, env));
		}
	}
	return(ret);
}

int			exec_not_build_not_pipe(t_list	*cmd, t_env *env)
{
	pid_t		pid;
	int		ret;

	ret = 0;
	pid = fork();
	cmd->pid = pid;
	cmd->is_fork = 1;
	if(cmd->pid == 0)
	{
		if(cmd->file)
			ft_dup_fd2(cmd->file);
		exit(exec_other(cmd, env));
	}
	return(ret);
}

int			exec_build_not_pipe(t_list	*cmd, t_env *env)
{
	int		ret;
	int		input;
	int		output;

	if(cmd->file)
	{
		input = dup(0);
		output = dup(1);
		pipe(cmd->pipe);
		if(dup2(cmd->pipe[1], 1) < 0)
			printf("\nErreur\n");
		if(dup2(cmd->pipe[0], 0) < 0)
			printf("\nErreur\n");
		ft_dup_fd2(cmd->file);
		ret = exec_build(cmd, env);
		close(cmd->pipe[0]);
		close(cmd->pipe[1]);
		close_fd(&(cmd->file));
		dup2(input, 0);
		dup2(output, 1);
		return(ret);
	}
	else
	{
		ret = exec_build(cmd, env);
	}
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
