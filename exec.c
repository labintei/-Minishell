/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 20:15:32 by labintei          #+#    #+#             */
/*   Updated: 2021/09/09 19:42:03 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* permet de savoir si la commande est un buuiltin
** ! RAJOUTER env !
*/

/* Je l ai peut etre deja ecrit quelque part mais je me perds dans les fichiers */
void	ft_dup_fd(t_list *cmd)
{
	t_list_file *f;

	f = cmd->file;
	if (f)
	{
		if (f->redir == '>' || f->redir == 'R')
			dup2(f->fd, 1);
		else if (f->redir == '<' || f->redir == 'L')
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
	if (cmd && cmd->type == '|')
	{
		if (dup2(cmd->pipe[1], 1) < 0)
			exit (EXIT_FAILURE);//Fonction de free et exit en cas d appel systeme qui 
	}	
	ft_dup_fd(cmd);
//	view_cmds(&cmd);
	return (1);
}

/* */
int		exec_bin(t_list *cmd, t_env *env)
{
	int ret;
	char **e;

	(void)cmd;
	(void)env;
	e = ft_env_string_tab(env);
	ret = execve(cmd->cmds[0], cmd->cmds, e);
	if (ret == 1)
	{
		exit (EXIT_FAILURE);//fonction erreur appel systeme
	}
	else
	{
		clear_tab(&e);
	}
	return (ret);
}

int	ma_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int		check_cmds_errors(t_list *elem, t_env *env)
{
	int	ret;

	(void)elem;
	(void)env;
	ret = 0;
	if (!elem->cmds[0])
		exit (EXIT_FAILURE);//revoir les erreurs
	if (ma_strcmp(elem->cmds[0], "..") == 0)
		printf("Erreur mauvaise commande 1\n");//a revoir
	else if (ma_strcmp(elem->cmds[0], ".") == 0)
		printf("Erreur mauvaise commande 2\n");//a revoir
	return (ret);
}

void	start_child_process(t_list *elem, t_env *env, bool builtin)
{
	(void)env;
	(void)builtin;
	//gerer les pipes au prealable


	if (!dup_pipes(elem))
	{
		exit (EXIT_FAILURE);
	}
	if (elem && elem->cmds && elem->cmds[0] && builtin)
		exit(exec_build(elem, env));
	//verif si les commandes sont mauvaises - a revoir
	check_cmds_errors(elem, env);
	//il faudrait que les fonctions renvient toutes un int
	if (elem && elem->cmds && elem->cmds[0] && !builtin)
	{
	//	printf("on va executer\n");
		exec_bin(elem, env);
	}
	//fermer les pipes
}

void	close_fd(t_list *elem)
{
	t_list_file *f;

	f = elem->file;
	//if ou while ? A revoir
	if (f)
	{
		if (f->redir != 'R')
			close(f->fd);
	}
}

/* va permettre de fermer les fd du file et fd de la cmd */
int		close_pipes(t_list *elem)
{
	(void)elem;
	/*
	t_list_file *f;

	close_fd(elem);
	*/
	//faire la suite
	return (0);
}

/* On exit puisqu'il s'agit du child */
/* a reprendre */
int		sub_exec_cmds(t_list *elem, t_env *env)
{
	int	ret;
	int	builtin;

	ret = 0;
	builtin = is_builtin(elem->cmds[0]);
//	printf("builtin is %i\n", builtin);
	if (is_piped(elem))
	{
		if (pipe(elem->pipe))
			exit (EXIT_FAILURE);//revoir fonction de sortie et exit mauvais syscall
	}
	//printf("Avant redir :\n");
	//printf("----------------------\n");
	//view_cmds(&env->cmds);
	if (is_redirected(elem))
	{
	//	printf("Redirection !\n");
		if (ft_redirection(env, elem) != 0)
			return (0);
	}
	//printf("Apres redir :\n");
	//printf("----------------------\n");
	//view_cmds(&env->cmds);
	elem->pid = fork();
	if (elem->pid == -1)
		exit (EXIT_FAILURE);//revoir erreur mauvais syscall
	//disable_signals(true);
	if (elem->pid == 0)
		start_child_process(elem, env, builtin);
	//faire la fonction de close_pipes
	close_pipes(elem);
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
		ret = find_exec_path(&(env->cmds->cmds[0]), env);
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
