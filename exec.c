/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 20:15:32 by labintei          #+#    #+#             */
/*   Updated: 2021/09/09 15:06:08 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* permet de savoir si la commande est un buuiltin
** ! RAJOUTER env !
*/

/* retourne la longueur de la liste chainee de var d'env */
int		list_env_len(t_list_env *env)
{
	int 		i;
	t_list_env	*elem;

	i = 0;
	elem = env;
	while (elem)
	{
		elem = elem->next;
		i++;
	}
	return (i);
}

/* A mettre dans un fichier utils */
void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	unsigned char	*buf_dest;
	unsigned char	*buf_src;

	buf_dest = (unsigned char *)dest;
	buf_src = (unsigned char *)src;
	if (dest == src)
		return (dest);
	while (size--)
	{
		*(buf_dest++) = *(buf_src++);
	}
	return (dest);
}


/* Je l ai peut etre deja ecrit quelque part mais je me perds dans les fichiers */
void	ft_dup_fd(t_list *cmd)
{
	t_list_file *f;

	f = cmd->file;
	if (f)
	{
		if (f->redir == '>' || f->redir == 'L')
			dup2(f->fd, 1);
		else if (f->redir == '<' || f->redir == 'R')
		{
			if (f->redir == '<')
			{
				dup2(f->fd, 0);
			}
			else
			{
				dup2(f->pipe_fd[0], 0);
				close(f->pipe_fd[0]);
			}
		}
	}
}

/* Demander a lauranne tom :
** On ne fait un dup que sur la commande qui porte le | ou aussi celle qui suit?
** on veut plutot faire des dup du file ? A tester
*/
int			dup_pipes(t_list *cmd)
{
	if (cmd->type && cmd->type == '|')
	{
		if (dup2(cmd->pipe[1], 1) < 0)
			exit (EXIT_FAILURE);//Fonction de free et exit en cas d appel systeme qui 
	}	
	ft_dup_fd(cmd);
//	view_cmds(&cmd);
	return (1);
}

int		exec_bin(t_list *cmd, t_env *env)
{
	int ret;

	(void)cmd;
	(void)env;

	ret = 0;
	return (ret);
}

int		check_cmds_errors(t_list *elem, t_env *env)
{
	int	ret;

	(void)elem;
	(void)env;
	ret = 0;

	if (!elem->cmds[0])
		exit (EXIT_FAILURE);//revoir les erreurs
	return (ret);
}

/* On exit puisqu'il s'agit du child */
/* a reprendre */
int		sub_exec_cmds(t_list *elem, t_env *env)
{
	int	ret;

	ret = 0;
	//gerer les pipes au prealable
	if (!dup_pipes(elem))
		exit (EXIT_FAILURE);
	//
	if (elem && elem->cmds && elem->cmds[0] && is_build(elem->cmds[0]))
		exit(exec_build(elem, env));
	//verif si les commandes sont mauvaises - a revoir
	check_cmds_errors(elem, env);
	//gerer les redir ensuite
	//disable les signaux
	//il faudrait que les fonctions renvient toutes un int
	
	else if(elem && elem->cmds && elem->cmds[0])
		exec_bin(elem, env);
	//fermer les pipes
	return (ret);
}

/* Nouvelle version d exec_cmds */
/* si il n y a pas de redirection alors t_list_file est nul */
/* Faire une condition pour appeler ft_redirection */
int		exec_cmds(t_env *env)
{
	int		ret;
	t_list	*elem;

	ret = 0;
	elem = env->cmds;
	while (elem)
	{
		//ret = ft_find_exec(cmds, env)
		ret = find_exec_path(&(env->cmds->cmds[0]), env);
		//voir retour d erreur
	//	printf("the exec path is %s\n", c->cmds[0]);
		//il faut que les fonctions d exec aient un retour 
		ret = sub_exec_cmds(elem, env);
		/* gestion des erreurs ameliorer 
		if (ret)
		{

		}*/
	//	wait_execution(elem, env);//commente car deja appele dans la fonction de Lauranne
		elem = elem->next;
	}
	ret = wait_execution(env->cmds, env);
	return (ret);
}

int		wait_execution(t_list *cmds, t_env *env)
{
	int 	ret;
	int 	status;

	ret = 0;
	(void)env;
	waitpid(cmds->pid, &status, 0);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	//last_error
	//revoir signaux
	//handle_signals();
	//faire les closes dans cette fonction
	return (ret);
}