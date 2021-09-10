/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 19:23:59 by labintei          #+#    #+#             */
/*   Updated: 2021/09/10 19:01:38 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* revoir les signaux pour le heredoc pour mieux coller au bash */
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
		line = ft_expansion(line, env, 0, NULL);
		free(temp);
		write(f->pipe_fd[1], line, ft_strlen(line));
		write(f->pipe_fd[1], "\n", 1);
		free(line);
	}
	close(f->pipe_fd[1]);
	return (0);
}

/* Redirection d'output simple
** Bash ouvre (essai) les fichiers. 
** Si l'un des open ne fonctionne pas c'est toute la commande qui est erronee.
** A priori il ne devrait y avoir qu un fichier donc pas besoin de boucler sur la liste de fichier.
** On ne peut pas donner le minimun de droits puisqu'il faut que d autres processus puissent modifier le fichier
*/
int			redir_output_simple(t_list *cmd)
{
	t_list_file *f;
	(void)cmd;

	//printf("Nous sommes ici\n");
	f = cmd->file;
	if (f && f->redir == '>')
	{
		//printf("\nThe path is %s\n", f->path);
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
	//printf("The resulting fd is %i\n", f->fd);
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

/* On va boucler commande par commande depuis la fonction appelante */
int		ft_redirection(t_env *env, t_list *cmd)
{
	if (redir_input_simple(cmd, env) != 0)
		return (-1);
	if (redir_output_simple(cmd) != 0)
		return (-1);
	return (0);
}
