/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 13:45:00 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 20:08:07 by labintei         ###   ########.fr       */
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
	int		j;
	struct	stat		si;

	g_ret = 0;
	if(c->pid == 0)
	{
		if(c->cmds && c->cmds[0] && c->cmds[0][0] && c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
		{
			j = open(c->cmds[0], O_RDONLY);
			if(fstat(j, &si) == -1)
			{
				ft_putstr_fd(c->cmds[0], 2);
				ft_putstr_fd(": command not found\n",2);
				g_ret = 1;
				exit(1);
			}
			ft_convert_env(&(env->env), &test);
			g_ret = execve(c->cmds[0], c->cmds, test);
			if(test)
				clear_tab(&test);
			exit(g_ret);
		}
		else if(c->cmds && c->cmds[0])
		{
			find_exec_path(&(c->cmds[0]), env);
			ft_convert_env(&(env->env), &test);
			g_ret = execve(c->cmds[0], c->cmds, test);
			if(test)
				clear_tab(&test);
			exit(g_ret);
		}
		exit(g_ret);
	}
	return(g_ret);
}

void		close_fd(t_list_file	**file)
{
	t_list_file		*temp;

	restart_t_list_file(file);
	temp = (*file);
	while(temp)
	{
		if(error_redirection(temp, 0))
			return ;
		if(temp->redir != 'L' && temp->fd > 0)
			close(temp->fd);
		if(temp->redir == 'L' && temp->fd > 0)
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

void	ft_dup_fd2(t_list_file *cmd, t_env *env)
{
	t_list_file		*temp;
	int				i;

	i = 0;
	temp = cmd;
	while(temp)
	{
		if(temp->redir == '>' || temp->redir == 'R')
			i = dup2(temp->fd, 1);
		else
		{
			if(temp->redir == '<')
			{
				i = dup2(temp->fd, 0);
			}
			else
			{
				i = dup2(temp->pipe_fd[0], 0);
				close(temp->pipe_fd[0]);
			}
		}
		if(i < 0)
			error_exec(3, env);
		temp = temp->next;
	}
}

int			wait_exec_cmds(t_list		*cmds)
{
	int			ret;
	int			status;
	int			i;

	i = 0;
	ret = 0;
	while(cmds)
	{
		if(cmds && cmds->cmds && cmds->is_fork)
		{
			i = 1;
			waitpid(cmds->pid, &status, 0);
			if(WIFEXITED(status))
				g_ret = WEXITSTATUS(status);
		}
		cmds = cmds->next;
	}
	if(i == 1)
		handle_signals();
	
	return(ret);
}

void		exec_cmd(t_list *cmd, t_env *env)
{
	int			is_piped;

	is_piped = 0;
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
		if(pipe(cmd->pipe))
			error_exec(1, env);
		is_piped = 1;
		cmd->is_piped = 1;
		g_ret = 1;
	}
//	if(cmd->error == 0 && cmd->file)
//	{
//		restart_t_list_file(&(cmd->file));
//		if(cmd->error == 0 && ft_redirection(cmd->file, env) == -1)
//			cmd->error = 3;
//		restart_t_list_file(&(cmd->file));
//	}
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
		exec_pipe(cmd, env, is_piped);
	else
	{
		if(!env->error && !cmd->error && cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
			exec_build_not_pipe(cmd, env);
		else if(!env->error && !cmd->error)
			exec_not_build_not_pipe(cmd, env);
	}
	close_pipes(cmd, is_piped);
}

void			exec_pipe(t_list *cmd, t_env *env, int is_piped)
{
	pid_t		pid;

	(void)is_piped;
	if((pid = fork()) < 0)
		return(error_exec(2, env));
	inhibit_signals(pid);
//	if(cmd->file)
//	{
//		restart_t_list_file(&(cmd->file));
//		ft_redirection(cmd->file, env);
//	}
	cmd->pid = pid;
	cmd->is_fork = 1;
	if(cmd->file && cmd->pid == 0)
	{
		restart_t_list_file(&(cmd->file));
		ft_redirection(cmd->file, env);
	}
	if(cmd->pid == 0)
	{
		if(cmd->type == '|'  && dup2(cmd->pipe[1], 1) < 0)
			error_exec(3, env);
		if(cmd->previous && cmd->previous->type == '|'  && dup2(cmd->previous->pipe[0], 0) < 0)
			error_exec(3, env);
//		if(cmd->file)
//		{
//			restart_t_list_file(&(cmd->file));
//			ft_redirection(cmd->file, env);
//		}
		if(cmd->file && !cmd->error)
			ft_dup_fd2(cmd->file, env);
		if(cmd->cmds && !cmd->error)
		{
			if(cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
				exit(g_ret = exec_build(cmd, env));
			else
				exit(exec_other(cmd, env));
		}
		else if(cmd->error)
			exit(g_ret = 1);
		else
			exit(g_ret = 0);
	}
}

void		exec_not_build_not_pipe(t_list	*cmd, t_env *env)
{
	pid_t		pid;

	if((pid = fork()) < 0)
		error_exec(2, env);
	inhibit_signals(pid);
//	restart_t_list_file(&(cmd->file));
//	ft_redirection(cmd->file, env);
	cmd->pid = pid;
	cmd->is_fork = 1;
	if(cmd->pid == 0)
	{
		if(cmd->file)
		{
			restart_t_list_file(&(cmd->file));
			ft_redirection(cmd->file, env);
		}
		if(cmd->file)
			ft_dup_fd2(cmd->file, env);
		exit(g_ret = exec_other(cmd, env));
	}
}

void			result(t_list_file *file, t_env *env, int *j, int pid)
{
	if(pid == 0)
	{
//		ft_redirection(file, env);
		exit(0);
	}
	else
		ft_redirection(file, env);
	return ;
}

void		exec_build_not_pipe(t_list	*cmd, t_env *env)
{
	int		input;
	int		output;
	int		pid;
	int		j;
	int		status;
	int		c;

	status = 0;
	if(cmd->file)
	{
		restart_t_list_file(&(cmd->file));
//		pid = fork();
//		inhibit_signals(pid);

		handle_signals_bis();
//		result(cmd->file, env, &j, pid);
		ft_redirection(cmd->file, env);
		waitpid(pid, &status, 0);
		restart_t_list_file(&(cmd->file));
		handle_signals();
//		printf("\nRET APRES LE SIGNAL %d %d \n", status, pid);
		input = dup(0);
		output = dup(1);
		pipe(cmd->pipe);
		if(dup2(cmd->pipe[1], 1) < 0)
			error_exec(3, env);
		if(dup2(cmd->pipe[0], 0) < 0)
			error_exec(3, env);
	//	pid = fork();
	//	inhibit_signals(pid);
	//	result(cmd->file, env, &j, pid);
	//	waitpid(pid, &status, 0);
	//	handle_signals();
	//	printf("\nRET APRES LE SIGNAL %d %d \n", status, pid);
		ft_dup_fd2(cmd->file, env);
		if(status != 2)
		{
			env->last_ret = exec_build(cmd, env);
		}
		close(cmd->pipe[0]);
		close(cmd->pipe[1]);
		close_fd(&(cmd->file));
		if(input < 0 || dup2(input, 0) < 0)
			error_exec(3, env);
		if(output < 0 || dup2(output, 1) < 0)
			error_exec(3, env);
	}
	else
		g_ret = exec_build(cmd, env);
}

int			exec_cmds(t_env *env)
{
	t_list		*c;
	int			ret;
	char		*s;
	char		*s2;

	ret = 0;
	list_cmds_restart(&(env->cmds));
	c = env->cmds;
	while(c)
	{
		if(c->error == 0)
			exec_cmd(c, env);
		env->last_ret = ret;
		c = c->next;
	}
	wait_exec_cmds(env->cmds);
	s = ft_itoa(g_ret);
	if(ft_find_env(&(env->env), "?"))
	{
		change_value(&(env->env), "?", s);
	}
	if(s)
		free(s);
	return(ret);
}
