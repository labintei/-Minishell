/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_bis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 10:48:23 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 15:50:38 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_heredoc_under(t_list_file *f, t_env *env)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			error_r(f->path);
			break ;
		}
		if (!ft_strcmp_1(line, f->path))
		{
			if (line)
				free(line);
			break ;
		}
		if (!(f->is_quotes))
			line = ft_expansion(line, env);
		write(f->pipe_fd[1], line, ft_strlen(line));
		write(f->pipe_fd[1], "\n", 1);
		if (line)
			free(line);
	}
	close(f->pipe_fd[1]);
	return (0);
}

void	if_fork_heredoc(t_list_file *f, t_env *env, int *pid)
{
	if (pipe(f->pipe_fd))
		error_exec(1, env);
	(*pid) = fork();
	inhibit_signals((*pid));
}

int	return_heredoc(int	*pid, int *status)
{
	waitpid((*pid), status, 0);
	handle_signals();
	if ((*status) == 2)
	{
		g_ret = 130;
		return (1);
	}
	return (0);
}

int	ft_heredoc(t_list_file *f, t_env *env, int is_fork)
{
	int		pid;
	int		status;

	if (pipe(f->pipe_fd))
			error_exec(1, env);
	if(!is_fork)
	{
		pid = fork();
		inhibit_signals(pid);
		if(pid == 0)
		{
			exit(ft_heredoc_under(f, env));
		}
		if(pid != 0)
		{
			waitpid((pid), &status, 0);
			close(f->pipe_fd[1]);
			handle_signals();
			if ((status) == 2)
			{
				g_ret = 130;
				return (1);
			}
			return (0);
		}
	}
	if(is_fork)
		return(ft_heredoc_under(f, env));
	return(0);
}
/*
int	ft_heredoc(t_list_file *f, t_env *env, int is_fork)
{
	int		pid;
	int		status;

	f->fd = 1;
	if (!is_fork)
	{
		if (pipe(f->pipe_fd))
			error_exec(1, env);
		(pid) = fork();
		inhibit_signals((pid));
		if (pid == 0)
		{
			ft_heredoc_under(f, env);
			exit(0);
		}
		waitpid((pid), &status, 0);
		handle_signals();
		if ((status) == 2)
		{
			g_ret = 130;
			return (1);
		}
	}
	if (is_fork)
	{
		if (pipe(f->pipe_fd))
			error_exec(1, env);
		ft_heredoc_under(f, env);
		return (0);
	}
	return (0);
}*/
