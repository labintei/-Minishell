/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 20:15:32 by labintei          #+#    #+#             */
/*   Updated: 2021/09/09 13:04:27 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_build(char *s)
{
	if(s && *s)
	{
		if(ft_strcmp(s, "echo") || ft_strcmp(s, "cd") || ft_strcmp(s, "pwd") \
		|| ft_strcmp(s, "unset") || ft_strcmp(s, "export") || \
		ft_strcmp(s, "exit"))
			return(1);
	}
	return(0);
}

int		exec_build(t_list	*a, t_env *env)
{

	bool	fork = 1;

	if(a->cmds && ft_strcmp(a->cmds[0], "cd"))
		return(cd(env->cmds, env->env, 0));
	if(a->cmds && ft_strcmp(a->cmds[0], "echo"))
		return(echo_build(env->cmds));
	if(a->cmds && ft_strcmp(a->cmds[0], "unset"))
		return(unset(env->cmds, env));
	if(a->cmds && ft_strcmp(a->cmds[0], "export"))
	{
//		printf("\nON est dans export\n");
		return(export_build(env->cmds, env/*, fork*/));
	}
	if(a->cmds && ft_strcmp(a->cmds[0], "pwd"))
		return(pwd(env));
	if(a->cmds && ft_strcmp(a->cmds[0], "env"))
		return(view_list_env(&(env->env)));
	if(a->cmds && ft_strcmp(a->cmds[0], "exit"))
		return(exit_build(env, a, fork));
	else
		return(1);
}


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

char	*flatten_env_var(t_list_env *env, t_env *mem)
{
	char 	*ret;
	int		i;
	int		total_len;

	ret = NULL;
	(void)mem;
	total_len = ft_strlen(env->var) + 1 + ft_strlen(env->val) + 1;
	ret = (char *)malloc(sizeof(char *) * total_len);
	//fonction de gestion des mauvais malloc
	i = ft_strlen(env->var);
	ret[i] = '=';
	ret[total_len - 1] = '\0';
	ft_memcpy(ret, env->var, ft_strlen(env->var));
	ft_memcpy(&(ret[i]), env->val, ft_strlen(env->val));
	return (ret);
}

char	**ft_env_string_tab(t_env *env)
{
	int 		i;
	t_list_env 	*elem;
	char		**ret;
	int			length;

	i = 0;
	length = list_env_len(env->env);
	ret = (char **)malloc(sizeof(char *) * (length + 1));
	//fonction de free exit proprement en cas d appel systeme rate
	ret[length] = NULL;
	elem = env->env;
	while (elem && i < length)
	{
		ret[i] = flatten_env_var(elem, env);
		//fonction de gestion des erreurs malloc
		i++;
		elem = elem->next;
	}
	return (ret);
}


int		exec_other(t_list	*c, t_env *env)
{
//	char	*path;
	char	**test;

//	c->pid = fork();
	if(c->cmds && c->cmds[0] && c->cmds[0][0] && c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
	{
		ft_convert_env(&(env->env), &test);
		execve(c->cmds[0], c->cmds, test);
		if(test)
			clear_tab(&test);
		return(0);
	}
	if(c->cmds && c->cmds[0])
	{
		find_exec_path(&(c->cmds[0]), env);
		ft_convert_env(&(env->env), &test);
		execve(c->cmds[0], c->cmds, test);
		if(test)
			clear_tab(&test);
	}
	return(0);
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

/* Demander a Lauranne ?
** est-ce qu on ajouterait pas.
*/
int			dup_pipes(t_list *cmd)
{
	ft_dup_fd(cmd);
	return (1);
}

int			exec_child(t_list *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
//	int	i;

//	i = 0;
	//Fonction de duplication des pipes
	if (!dup_pipes(cmd))
		exit (EXIT_FAILURE);//revoir fonction de sortie 
	//fonction qui verifie si c est un builtin puis l execute
	//fonction qui verifie si la commande est null
	//Autre fonction sur les pipes ?
	//fonctions qui verifient que la commande n est pas un ".." ou "."
	//fonction qui execute les commandes non builtin et verifie les erreurs
	//attention absolute ou relatif path ?
	char	**e;

	e = ft_env_string_tab(env);
	/*
	while (e[i])
	{
		printf("%s\n", e[i]);
		i++;
	}
	*/
	if (execve(cmd->cmds[0],cmd->cmds, e) == -1)
	{
		//gestion des erreurs
		printf("erreur\n");
	}
	clear_tab(&e);
	exit (EXIT_FAILURE);
}


int			exec_cmd(t_list *cmd, t_env *env)
{
	pid_t		pid;
	int			ret;
//	int			status;
//separer les instructions relatives aux pipes 
//	int			pipe_open;

	ret  = 1;
//	pipe_open = 0;
//	printf("Nous allons executer la commande\n");
	/* replacer la gestion des pipes ailleurs pour la norme */
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
//		pipe_open = 1;
		if(pipe(cmd->pipe))
			printf("\nERREUR EXIT FATAL\n");
	}
	//verifier qu il n y a 
	if (cmd->type == 'R' || cmd->type == 'L' || cmd->type == '<' || cmd->type == '>')
		ft_redirection(env, cmd);//revoir retour apres
	//	return (0);//revoir les retours
	pid = fork();
	cmd->pid = pid;//Attention vaut mieux passer par la structure ?
	/* revoir gestion des erreurs
	if(pid < 0)
		printf("\nERREUR EXIT FATAL\n");
	*/

	if(pid == 0 && cmd->pid == 0)
	{
		//gestion des dup2 separee
		if(cmd->type == '|' && dup2(cmd->pipe[1], 1) < 0)
			printf("\nERREUR EXIT FATAL\n");
		if(cmd->previous && cmd->previous->type == '|' && dup2(cmd->previous->pipe[0], 0) < 0)
			printf("\nERREUR EXIT FATAL\n");
		if(cmd->cmds)//cmds && cmd->cmds[0]
		{
			ret = exec_child(cmd, env);
			//ret = exec_other(cmd, env);//correspondrait au exec_build ? changer de nom ?
		}
			
	//	exit(ret);//pourquoi exit ? pour que le child se ferme comme ca?
	}
	//Fonction de wait et de close a deplacer dans la fonction appelante ?
	/*
	else
	{
		waitpid(pid, &status, 0);
		if(pipe_open)
		{
			close(cmd->pipe[1]);
			if(!cmd->next)
				close(cmd->pipe[0]);
		}
		if(cmd->previous && cmd->previous->type == '|')
			close(cmd->previous->pipe[0]);
		if(WIFEXITED(status))
			ret = WEXITSTATUS(status);
	}
	*/
	return(ret);
}

int		sub_exec_cmds(t_list *elem, t_env *env)
{
	int	ret;

	ret = 0;
	//gerer les pipes au prealable
	//gerer les redir ensuite
	//disable les signaux
	//il faudrait que les fonctions renvient toutes un int
	if(elem && elem->cmds && elem->cmds[0] && is_build(elem->cmds[0]))
		exec_build(elem, env);
	else if(elem && elem->cmds && elem->cmds[0])
		exec_cmd(elem, env);
	//fermer les pipes
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


void		exec_cmds_old(t_env *env)
{
	t_list	*c;
//	int	status;
//	int	ret;

	list_cmds_restart(&(env->cmds));
//	printf("\nAAAAAAA\n");
	c = env->cmds;
	while(env->cmds)
	{
		c = env->cmds;
		if(c && c->cmds && c->cmds[0] && is_build(c->cmds[0]))
			exec_build(c, env);
		else if(c && c->cmds && c->cmds[0])
			exec_cmd(c, env);
		if(!(env->cmds)->next)
			break ;
/*		if(c && c->cmds && c->cmds[0] && is_build(c->cmds[0]))
			exec_build(c, env);
		else
			exec_other(c, env);*/
		env->cmds = env->cmds->next;
	}
/*	c = env->cmds;
	while(c)
	{
		if(c->pid)
			waitpid(c->pid, &status, 0);
		c  = c->next;
	}*/
}
/*
void		exec_cmds(t_env *env)
{
	t_list	*c;
	int		status;
	int		ret;

	status = 0;
	list_cmds_restart(&(env->cmds));
	c = env->cmds;
	while(c)
	{
		if(c && c->cmds && c->cmds[0] && is_build(c->cmds[0]))
			ret = exec_build(c, env);
		else
			ret = exec_other(c, env);
		c = c->next;
	}
	c = env->cmds;
	while(c)
	{
		if(c->pid)
			waitpid(c->pid, &status, 0);
		c  = c->next;
	}
}
*/
