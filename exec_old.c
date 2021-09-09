/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:39:06 by malatini          #+#    #+#             */
/*   Updated: 2021/09/09 17:17:57 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Sauvegarde de l ancien code separee pour plus de lisibilite */
#include "minishell.h"

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
	if (cmd->file)
	{
	//	printf("nous sommes dans le cas d'une redir\n");
		ft_redirection(env, cmd);//revoir retour apres
	}
	view_cmds(&cmd);
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
		//reprendre fonction is_build de lauranne
		if(c && c->cmds && c->cmds[0] && is_builtin(c->cmds[0]))
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