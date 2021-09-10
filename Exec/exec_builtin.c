/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:45:22 by malatini          #+#    #+#             */
/*   Updated: 2021/09/10 19:00:50 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//rajout de env par mahaut 
int			is_build(char *s)
{
	if (s && *s)
	{
		if (ma_strcmp(s, "echo") || ma_strcmp(s, "cd") || ma_strcmp(s, "pwd") \
		|| ma_strcmp(s, "unset") || ma_strcmp(s, "export") \
		|| ma_strcmp(s, "exit") || ma_strcmp(s, "env"))
			return(1);
	}
	return(0);
}

/* Fonction qui va permettre d executer les builtins */
int		exec_build(t_list	*a, t_env *env)
{

	bool	fork = 1;
	if (a->cmds && !ma_strcmp(a->cmds[0], "cd"))
		return (cd(env->cmds, env->env, 1));
	else if (a->cmds && !ma_strcmp(a->cmds[0], "echo"))
		return (echo_build(env->cmds));
	else if (a->cmds && !ma_strcmp(a->cmds[0], "unset"))
		return (unset(env->cmds, env));
	else if (a->cmds && !ma_strcmp(a->cmds[0], "export"))
	{
		return (export_build(env->cmds, env));
	}
	else if (a->cmds && !ma_strcmp(a->cmds[0], "pwd"))
		return (pwd(env));
	else if (a->cmds && !ma_strcmp(a->cmds[0], "env"))
		return (view_list_env(&(env->env)));//Lauranne: verifier qu il n y a pas d autre option
	else if (a->cmds && !ma_strcmp(a->cmds[0], "exit"))
		return (exit_build(env, a, fork));
	return (0);
}
