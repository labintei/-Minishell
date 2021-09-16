/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:39:06 by malatini          #+#    #+#             */
/*   Updated: 2021/09/16 10:23:54 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Sauvegarde de l ancien code separee pour plus de lisibilite */
#include "../minishell.h"
/*
int		exec_other(t_list	*c, t_env *env)
{
	char	**test;
	int		ret;

	if(c->is_fork == 0)
	{
		c->pid = fork();
	}
	ret = 0;
	if(c->pid == 0)
	{
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
	}
	return(ret);
}*/
