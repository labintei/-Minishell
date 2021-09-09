/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:45:22 by malatini          #+#    #+#             */
/*   Updated: 2021/09/09 17:13:49 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//rajout de env par mahaut 
int			is_build(char *s)
{
	if(s && *s)
	{
		if(ft_strcmp(s, "echo") || ft_strcmp(s, "cd") || ft_strcmp(s, "pwd") \
		|| ft_strcmp(s, "unset") || ft_strcmp(s, "export") || \
		ft_strcmp(s, "exit") || ft_strcmp(s, "env"))
			return(1);
	}
	return(0);
}

/* Fonction qui va permettre d executer les builtins */
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
