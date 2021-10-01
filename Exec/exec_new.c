/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 13:45:00 by labintei          #+#    #+#             */
/*   Updated: 2021/10/01 16:56:12 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_o(char ***test, t_env *env, t_list *c)
{
	ft_convert_env(&(env->env), test);
	g_ret = execve(c->cmds[0], c->cmds, (*test));
	if (test)
		clear_tab(test);
	return(g_ret);
}

int	erreur_o(t_list	*c)
{
	ft_putstr_fd(c->cmds[0], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	g_ret = 1;
	return (1);
}

int	exec_other(t_list	*c, t_env *env)
{
	char			**test;
	int				j;
	struct stat		si;

	g_ret = 0;
	if (c->pid == 0)
	{
		if (c->cmds && c->cmds[0] && c->cmds[0][0] && \
		c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
		{
			j = open(c->cmds[0], O_RDONLY);
			if (fstat(j, &si) == -1)
				exit(erreur_o(c));
			exec_o(&(test), env, c);
		}
		else if (c->cmds && c->cmds[0])
		{
			j = open(c->cmds[0], O_RDONLY);
			if (fstat(j, &si) == -1)
				find_exec_path(&(c->cmds[0]), env);
			exec_o(&(test), env, c);
		}
	}
	exit(g_ret);
	return (g_ret);
}

void	close_fd(t_list_file	**file)
{
	t_list_file		*temp;

	restart_t_list_file(file);
	temp = (*file);
	while (temp)
	{
		if (error_redirection(temp, 0))
			return ;
		if (temp->redir != 'L' && temp->fd > 0)
			close(temp->fd);
		if (temp->redir == 'L' && temp->fd > 0)
		{
			close(temp->pipe_fd[0]);
			close(temp->pipe_fd[1]);
		}
		temp = temp->next;
	}
}

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
	while (temp && temp->fd != -2)
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

int	wait_exec_cmds(t_list	*cmds)
{
	int			ret;
	int			status;

	ret = 0;
	while (cmds)
	{
		if (cmds && cmds->cmds && cmds->is_fork)
		{
			waitpid(cmds->pid, &status, 0);
			if (WIFEXITED(status))
				g_ret = WEXITSTATUS(status);
		}
		cmds = cmds->next;
	}
	handle_signals();
	return (ret);
}

void	exec_cmd(t_list *cmd, t_env *env)
{
	int	is_piped;

	is_piped = 0;
	if (cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
		if (pipe(cmd->pipe))
			error_exec(1, env);
		is_piped = 1;
		cmd->is_piped = 1;
		g_ret = 1;
	}
	if (cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
		exec_pipe(cmd, env, is_piped);
	else
	{
		if (!env->error && !cmd->error && \
		cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
			exec_build_not_pipe(cmd, env);
		else if (!env->error && !cmd->error)
			exec_not_build_not_pipe(cmd, env);
	}
	close_pipes(cmd, is_piped);
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

void	exec_pipe(t_list *cmd, t_env *env, int is_piped)
{
	pid_t		pid;
	int			status;

	(void)is_piped;
	status = 0;
	pid = fork();
	cmd->pid = pid;
	cmd->is_fork = 1;
	if (cmd->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (cmd->file)
		{
			restart_t_list_file(&(cmd->file));
			if (cmd->error == 0 && ft_redirection(cmd->file, env, 1) == -1)
				cmd->error = 3;
			restart_t_list_file(&(cmd->file));
		}
		if (cmd->type == '|' && dup2(cmd->pipe[1], 1) < 0)
			error_exec(3, env);
		if (cmd->previous && cmd->previous->type == '|' \
		&& dup2(cmd->previous->pipe[0], 0) < 0)
			error_exec(3, env);
		if (cmd->file)
		{
			restart_t_list_file(&(cmd->file));
			ft_dup_fd2(cmd->file);
		}
		if (cmd->cmds && !env->cmds->error)
		{
			if (cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
				exit(exec_build(cmd, env));
			else
				exit(exec_other(cmd, env));
		}
		else if (env->cmds->error)
			exit(1);
		else
			exit(0);
	}
	if (wait_heredoc(&(cmd->file)))
	{
		waitpid(cmd->pid, &status, 0);
		if (status == 2)
			g_ret = 130;
	}
}

int	exec_not_build_not_pipe(t_list	*cmd, t_env *env)
{
	pid_t		pid;

	pid = fork();
	inhibit_signals(pid);
	cmd->pid = pid;
	cmd->is_fork = 1;
	if (pid == 0)
	{
		if (cmd->file)
		{
			restart_t_list_file(&(cmd->file));
			if (cmd->error == 0 && ft_redirection(cmd->file, env, 1) == -1)
				cmd->error = 3;
			restart_t_list_file(&(cmd->file));
			ft_dup_fd2(cmd->file);
		}
		if (cmd->error != 3)
			exit(exec_other(cmd, env));
		else
			exit(1);
	}
	return (g_ret);
}

int	exec_build_not_pipe(t_list	*cmd, t_env *env)
{
	int		input;
	int		output;

	if (cmd->file)
	{
		restart_t_list_file(&(cmd->file));
		if (cmd->error == 0 && ft_redirection(cmd->file, env, 1) == -1)
		{
			g_ret = 1;
			cmd->error = 3;
		}
		restart_t_list_file(&(cmd->file));
		input = dup(0);
		output = dup(1);
		pipe(cmd->pipe);
		if (dup2(cmd->pipe[1], 1) < 0)
			error_exec(3, env);
		if (dup2(cmd->pipe[0], 0) < 0)
			error_exec(3, env);
		if (cmd->file)
			ft_dup_fd2(cmd->file);
		if (g_ret != 130 && cmd->error != 3)
			g_ret = exec_build(cmd, env);
		close(cmd->pipe[0]);
		close(cmd->pipe[1]);
		close_fd(&(cmd->file));
		if (input < 0 || dup2(input, 0) < 0)
			error_exec(3, env);
		if (output < 0 || dup2(output, 1) < 0)
			error_exec(3, env);
	}
	else
		g_ret = exec_build(cmd, env);
	return (g_ret);
}

int	exec_cmds(t_env *env)
{
	t_list		*c;
	char		*s;

	g_ret = 0;
	list_cmds_restart(&(env->cmds));
	c = env->cmds;
	signal(SIGINT, SIG_IGN);
	while (c && g_ret != 130)
	{
		if (c->error == 0)
			exec_cmd(c, env);
		c = c->next;
	}
	wait_exec_cmds(env->cmds);
	s = ft_itoa(g_ret);
	if (ft_find_env(&(env->env), "?"))
		change_value(&(env->env), "?", s);
	if (s)
		free(s);
	return (g_ret);
}
