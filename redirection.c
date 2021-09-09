/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 19:23:59 by labintei          #+#    #+#             */
/*   Updated: 2021/09/09 14:36:12 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Cette etape de redirection est une etape preliminaire a la commande
** On va noter la valeur du fd et des pipes ensuite on executera */

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

/* retourne la valeur d'une cle (variable d'environnement)
** Necessaire pour l'expansion.
*/
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

/* A tester - fonction deja faite 
** Fonction principale qui gere l expansion.
?*/
char *ft_expansion(char *str, t_env *env, int i, char *read)
{
	char 	*key;
	char	*value;
	int		j;

	(void)env;
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

	(void)f;
	pipe(f->pipe_fd);
	while (true)
	{
		line = readline("> ");
		if (!line)
			break;
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

/* 
void		redir_output_append()
{
	close(1);
	open("doc", O_RDWR | O_CREAT | O_APPEND, 0644);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}
*/

/* Redirection d'output simple
** Bash ouvre (essai) les fichiers. 
** Si l'un des open ne fonctionne pas c'est toute la commande qui est erronee.
** A priori il ne devrait y avoir qu un fichier donc pas besoin de boucler sur la liste de fichier.
** On ne peut pas donner le minimun de droits puisqu'il faut que d autres processus puissent modifier le fichier
** 
*/
int			redir_output_simple(t_list *cmd)
{
	t_list_file *f;
	(void)cmd;

	f = cmd->file;
	if (f && f->redir == '<')
	{
		//printf("le path est %s\n", f->path);
		f->fd = open(f->path, O_RDWR | O_CREAT | O_TRUNC, 0644);
		//Rajouter quand on aura fait les messages d erreurs
		/*
		if (errno == EACCES)
			return (display_error(2));
		*/
	}
	else if (f && f->redir == 'R')
	{
		f->fd = open(f->path, O_CREAT | O_RDWR | O_APPEND, 0644);
		//erreur errno
	}
	return (0);
}
	

int		redir_input_simple(t_list *cmd, t_env *env)
{
	(void)cmd;
	(void)env;

	t_list_file *f;

	f = cmd->file;
	if (f && f->redir == '<')
	{
		f->fd = open(f->path, O_RDONLY);
		/* erreur errno */
		if (f->fd < 0)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(f->path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
			//Ici le process child devrait s arreter
		}
	}
	else if (f && f->redir == 'L')
	{
		ft_heredoc(f, env);
	}
	return (0);
}

/* On va boucler commande par commande depuis la fonction appelante */
int		ft_redirection(t_env *env, t_list *cmd)
{
	if (redir_input_simple(cmd, env) != 0)
		return (-1);
	if (redir_output_simple(cmd) != 0)
		return (-1);
	return (0);
}


