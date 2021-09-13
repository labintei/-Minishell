/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:39:06 by malatini          #+#    #+#             */
/*   Updated: 2021/09/13 16:06:45 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Sauvegarde de l ancien code separee pour plus de lisibilite */
#include "../minishell.h"

int		exec_other(t_list	*c, t_env *env)
{
	char	**test;
	int		ret;

	ret = 0;
	if(c->cmds && c->cmds[0] && c->cmds[0][0] && c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
	{
		ft_convert_env(&(env->env), &test);
		ret = execve(c->cmds[0], c->cmds, test);
		if(test)
			clear_tab(&test);
		return(ret);
	}
	if(c->cmds && c->cmds[0])
	{
		find_exec_path(&(c->cmds[0]), env);
		ft_convert_env(&(env->env), &test);
		ret = execve(c->cmds[0], c->cmds, test);
		if(test)
			clear_tab(&test);
	}
	return(ret);
}

/*
int			exec_child(t_list *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
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
	while (e[i])
	{
		printf("%s\n", e[i]);
		i++;
	}
	if (execve(cmd->cmds[0],cmd->cmds, e) == -1)
	{
		//gestion des erreurs
		printf("erreur\n");
	}
	clear_tab(&e);
	exit (EXIT_FAILURE);
}*/
/*
void		exec_cmds_old(t_env *env)
{
	t_list	*c;

	list_cmds_restart(&(env->cmds));
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
		env->cmds = env->cmds->next;
	}
}*/
