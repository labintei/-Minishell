/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:40:31 by labintei          #+#    #+#             */
/*   Updated: 2021/10/06 18:16:12 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_build(char *s)
{
	if (s && *s)
	{
		if (ft_strcmp_1(s, "echo") || ft_strcmp_1(s, "cd") || \
		ft_strcmp_1(s, "pwd") \
		|| ft_strcmp_1(s, "unset") || ft_strcmp_1(s, "export") \
		|| ft_strcmp_1(s, "exit") || ft_strcmp_1(s, "env"))
			return (1);
	}
	return (0);
}

int	exec_build(t_list	*a, t_env *env)
{
	if (a->cmds && !ft_strcmp_1(a->cmds[0], "cd"))
		return (cd(a, env->env, a->is_fork));
	else if (a->cmds && !ft_strcmp_1(a->cmds[0], "echo"))
		return (echo_build(a));
	else if (a->cmds && !ft_strcmp_1(a->cmds[0], "unset"))
		return (unset(a, env));
	else if (a->cmds && !ft_strcmp_1(a->cmds[0], "export"))
		return (export_build(a, env));
	else if (a->cmds && !ft_strcmp_1(a->cmds[0], "pwd"))
		return (pwd(env));
	else if (a->cmds && !ft_strcmp_1(a->cmds[0], "env"))
		return (view_list_env(&(env->env)));
	else if (a->cmds && !ft_strcmp_1(a->cmds[0], "exit"))
		return (exit_build(env, a, a->is_fork));
	return (0);
}
