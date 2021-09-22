/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:42:29 by labintei          #+#    #+#             */
/*   Updated: 2021/09/22 15:24:12 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_heredoc(t_list_file *f, t_env *env)
{
	char	*line;
//	char	*temp;

	pipe(f->pipe_fd);
	while (true)
	{
		line = readline("> ");
		if (!line)
			break;
		if (!ma_strcmp(line, f->path))
		{
			free(line);
			break ;
		}
//		temp = line;
		line = ft_expansion(line, env);
//		if(temp)
//			free(temp);
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
		f->fd = open(f->path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	}
	else if (f && f->redir == 'R')
	{
		f->fd = open(f->path, O_CREAT | O_RDWR | O_APPEND, 0644);
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
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(f->path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
	}
	else if (f && f->redir == 'L')
		ft_heredoc(f, env);
	return (0);
}

int		ft_redirection(t_list_file		*file, t_env *env)
{
	t_list_file		*new;

	new = file;
	while(new)
	{
		if (redir_input_simple(new, env) != 0)
			return (-1);
		new = new->next;
	}
	new = file;
	while(new)
	{
		if (redir_output_simple(new) != 0)
			return (-1);
		new = new->next;
	}
	return (0);
}
