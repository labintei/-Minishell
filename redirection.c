/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 19:23:59 by labintei          #+#    #+#             */
/*   Updated: 2021/09/08 13:25:51 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* A remettre dans des fichiers utiles */
char	*ft_strnewcat(char *first, char *second)
{
	int 	i;
	int 	j;
	char	*new;
	int		len_s;
	
	len_s = ft_strlen(second);
	new = (char *)malloc(ft_strlen(first) + len_s);
	if (!new)
		return (NULL);
	i = 0;
	while (first && first[i])
	{
		new[i] = first[i];
		i++;
	}
	j = 0;
	while (j < len_s)
	{
		new[i + j] = second[j];
		j++;
	}
	new[i + j] = '\0';
	if (first)
		free(first);
	return (new);
}


char	*get_value_from_key(char *str, int *i)
{
	char 	*buffer;
	int		j;

	buffer = (char *)malloc(sizeof(char));
	j = 1;
	if (!buffer)
	//fonction d erreur en cas de mauvais malloc
		return (NULL);
	buffer[0] = 0;
	if (str[j] == '?')
	{
		*i += 2;
		return (ft_strnewcat(buffer, "?"));
	}
	while (buffer[j])
	{
		if (!is_alphanum(buffer[j]))
		{
			*i += j;
			return (buffer);
		}
		buffer = ft_strnewcat(buffer, &str[j]);
	}
	*i += j;
	return (buffer);
}

/* A tester - fonction deja faite ?*/
char *ft_expansion(char *str, t_env *env, int i, char *read)
{
	char 	*key;
	char	*value;
	int		j;

	while (str[i])
	{
		if (str[i] == '$')
		{
			j = 0;
			key = get_value_from_key(str + i, &j);
			if (ft_strlen(key) > 0)
			{
				i += j;
				value = ft_strnewcat(read, value);
				free(key);
				key = NULL;
			}
			if (key)
				free(key);
			read = ft_strnewcat(read, &str[i++]);
		}
		i++;
	}
	return (read);
}

/* revoir les signaux pour le heredoc pour mieux coller au bash */
void	ft_heredoc(t_list_file *f, t_env *env)
{
	char	*line;
	char	*temp;

	pipe(f->pipe_fd);
	while (true)
	{
		line = readline("> ");
		if (!line)
			break;
		//revoir comment savoir si les redirections etaient entre quotes ou pas
		//if (!f->quote)
			temp = line;
			line = ft_expansion(line, env, 0, NULL);
			free(temp);
		write(f->pipe_fd[1], line, ft_strlen(line));
		write(f->pipe_fd[1], "\n", 1);
		free(line);
	}
	close(f->pipe_fd[1]);
	//Pourquoi tom/thomas n ont pas ferme le deuxieme pipe ?
	//close(f->pipe_fd[0]);
}


void		ft_redirection(t_env *env)
{
	(void)env;
}

/*
void		redir_output_append()
{
	close(1);
	open("doc", O_RDWR | O_CREAT | O_APPEND, 0644);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}


void		redir_output_simple()
{
	close(1);
	open("doc", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}

void		redir_input_simple()
{
	close(0);
	open("doc", O_RDONLY);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}*/
