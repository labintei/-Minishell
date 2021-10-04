/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 13:45:00 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 16:00:47 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_exec_build(t_list *cmd, t_env *env)
{
	exit(exec_build(cmd, env));
}

int		exec_pipe(t_list *cmd, t_env *env)
{
//	if (cmd->file)
//	{
//		restart_t_list_file(&(cmd->file));
//		if (cmd->error == 0 && ft_redirection(cmd->file, env, 1) == -1)
//		{
//			cmd->error = 3;
//		}
//	}
	if (cmd->type == '|' && dup2(cmd->pipe[1], 1) < 0)
		exit(1);
	if (cmd->previous && cmd->previous->type == '|' \
	&& dup2(cmd->previous->pipe[0], 0) < 0)
		exit(1);
	if (cmd->file)
		ft_dup_fd2(cmd->file);
	if (cmd->type == '|')
		close(cmd->pipe[0]);
	if (g_ret != 130 && cmd->cmds && !env->cmds->error)
	{
		if (cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]))
			return(exec_build(cmd, env));
		else
			return(exec_other(cmd, env));
	}
	else if (g_ret != 0)
		return(g_ret);
	else
		return(0);
}

void	put_return_var(t_env *env)
{
	char	*s;

	s = ft_itoa(g_ret);
	if (ft_find_env(&(env->env), "?"))
		change_value(&(env->env), "?", s);
	if (s)
		free(s);
}

int	exec_cmds(t_env *env)
{
	t_list		*c;
	t_list		*a;

	g_ret = 0;
	list_cmds_restart(&(env->cmds));
	if (env->cmds && !env->cmds->next)
		g_ret = exec_one_cmd(env->cmds, env);
	else
	{
		c = env->cmds;
		a = env->cmds;
		while (c && g_ret != 130)
		{
			if (c->error == 0)
				exec_cmd(c, env);
			c = c->next;
		}
		wait_exec_cmds(a);
	}
	put_return_var(env);
	return (g_ret);
}
