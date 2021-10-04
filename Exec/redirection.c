/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:42:29 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 10:53:03 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_r(char *s)
{
	ft_putstr_fd("minishell: warning: here-document delimited ", 2);
	ft_putstr_fd("by end-of-file (wanted '", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("')\n", 2);
}
/*
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

int	ft_heredoc(t_list_file *f, t_env *env, int is_fork)
{
	int		pid;
	int		status;

	if (!is_fork)
	{
		if (pipe(f->pipe_fd))
			error_exec(1, env);
		pid = fork();
		inhibit_signals(pid);
		if (pid == 0)
		{
			ft_heredoc_under(f, env);
			exit(0);
		}
		waitpid(pid, &status, 0);
		handle_signals();
		if (status == 2)
		{
			g_ret = 130;
			return (1);
		}
		return (0);
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

int	redir_output_simple(t_list_file *f)
{
	if (f && f->redir == '>')
		f->fd = open(f->path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (f && f->redir == 'R')
		f->fd = open(f->path, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (f->fd < 0 && ((f->redir == 'R' || f->redir == '>')))
	{
		ft_putstr_fd("Failed to open fd\n", 2);
		g_ret = 1;
		return (1);
	}
	return (0);
}

int	redir_input_simple(t_list_file	*f, t_env *env, int is_fork)
{
	if (f && f->redir == '<')
	{
		f->fd = open(f->path, O_RDONLY);
		if (f->fd < 0)
		{
			ft_putstr_fd("minishell : ", 2);
			ft_putstr_fd(f->path, 2);
			ft_putstr_fd("No such file or directory\n", 2);
			g_ret = 1;
			return (1);
		}
	}
	else if (f && f->redir == 'L')
		return (ft_heredoc(f, env, is_fork));
	return (0);
}

int	error_redirection(t_list_file *f)
{
	if (f && f->redir != 'L' && (f->path && ft_strlen(f->path) > 255))
	{
		ft_putstr_fd(f->path, 2);
		ft_putstr_fd(": File name too long\n", 2);
		g_ret = 1;
		return (1);
	}
	if (f && f->redir != 'L' && f->ambigous)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(f->ambigous, 2);
		ft_putstr_fd(" ambigous redirect\n", 2);
		g_ret = 1;
		return (1);
	}
	if (f && (!f->path || !f->path[0]))
	{
		g_ret = 1;
		ft_putstr_fd("minishell : No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int	ft_redirection(t_list_file	*file, t_env *env, int is_fork)
{
	t_list_file		*new;

	new = file;
	while (new)
	{
		if (error_redirection(file))
			return (-1);
		if (redir_input_simple(new, env, is_fork) != 0)
			return (-1);
		if (redir_output_simple(new) != 0)
			return (-1);
		new = new->next;
	}
	return (0);
}
