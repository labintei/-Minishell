/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:42:29 by labintei          #+#    #+#             */
/*   Updated: 2021/09/29 15:21:35 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_heredoc(t_list_file *f, t_env *env)
{
	char	*line;
	int		ret;

	if(pipe(f->pipe_fd))
		error_exec(1, env);
	while (1 && g_ret != 130)
	{
		line = readline("> ");
		if(g_ret == 130)
		{
			if(line)
				free(line);
			break ;
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted '",2);
			ft_putstr_fd(f->path, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		if (!ma_strcmp(line, f->path))
		{
			free(line);
			break ;
		}
		if(!(f->is_quotes))
			line = ft_expansion(line, env);
		write(f->pipe_fd[1], line, ft_strlen(line));
		write(f->pipe_fd[1], "\n", 1);
		if(line)
			free(line);
	}
	close(f->pipe_fd[1]);
	return (0);
}

int			redir_output_simple(t_list_file *f)
{
	if (f && f->redir == '>')
	{
		if((f->fd = open(f->path, O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)
		{
			ft_putstr_fd("Failed to open fd\n", 2);
			g_ret = 1;
			return(1);
		}
	}
	else if (f && f->redir == 'R')
	{
		if((f->fd = open(f->path, O_CREAT | O_RDWR | O_APPEND, 0644)) < 0)
		{
			ft_putstr_fd("Failed to open fd\n", 2);
			g_ret = 1;
			return(1);
		}
	}
	return (0);
}

int		redir_input_simple(t_list_file	*f, t_env *env)
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
	{
		ft_heredoc(f, env);
	}
	return (0);
}

int		error_redirection(t_list_file	*f, char y)
{
	if(f && f->redir != 'L' && (f->path && ft_strlen(f->path) > 255))
	{
		if(y == 1)
		{
			ft_putstr_fd(f->path, 2);
			ft_putstr_fd(": File name too long\n", 2);
			g_ret = 1;
		}
		return(1);
	}
	if(f && f->redir != 'L' && f->ambigous)
	{
		if(y == 1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(f->ambigous, 2);
			ft_putstr_fd(" ambigous redirect\n", 2);
			g_ret = 1;
		}
		return(1);
	}
	if(f && (!f->path || !f->path[0]))
	{
		if(y == 1)
		{
			g_ret = 1;
			ft_putstr_fd("minishell : No such file or directory\n", 2);
		}
		return(1);
	}

	return(0);
}

int		ft_redirection(t_list_file		*file, t_env *env)
{
	t_list_file		*new;

//	if(pid != 0)
//		return(0);
	new = file;
	while(new)
	{
		if(error_redirection(file, 1))
			return(-1);
		if (redir_input_simple(new, env) != 0)
			return (-1);
		if (redir_output_simple(new) != 0)
			return (-1);
		new = new->next;
	}
//	*j = 4;
	return (0);
}
