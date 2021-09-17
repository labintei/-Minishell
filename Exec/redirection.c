/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:42:29 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 18:07:41 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_heredoc(t_list_file *f, t_env *env)
{
	char	*line;
	char	*temp;

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
		temp = line;
		printf("\n%s\n", line);
		line = ft_expansion(line, env, 0, NULL);
		printf("\n%s\n", line);
		free(temp);
		write(f->pipe_fd[1], line, ft_strlen(line));
		write(f->pipe_fd[1], "\n", 1);
		free(line);
	}
	close(f->pipe_fd[1]);
	return (0);
}

int			redir_output_simple(t_list *cmd)
{
	t_list_file *f;

	f = cmd->file;
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

int		redir_input_simple(t_list *cmd, t_env *env)
{
	t_list_file *f;

	f = cmd->file;
	if (f && f->redir == '<')
	{
		f->fd = open(f->path, O_RDONLY);
		/* erreur errno */
		printf("The fd is %i\n", f->fd);
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

int		ft_redirection(t_env *env, t_list *cmd)
{
	if (redir_input_simple(cmd, env) != 0)
		return (-1);
	if (redir_output_simple(cmd) != 0)
		return (-1);
	return (0);
}
