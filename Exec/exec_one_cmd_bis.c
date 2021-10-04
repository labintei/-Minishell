/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:10:03 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 11:13:42 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_pid_built(t_list *cmd, t_env *env)
{
	if (g_ret != 130 && cmd->cmds && (ft_strcmp(cmd->cmds[0], "echo") || \
	(ft_strcmp(cmd->cmds[0], "export") && !cmd->cmds[1]) || \
	(ft_strcmp(cmd->cmds[0], "env"))))
	{
		if (cmd->file)
			ft_dup_fd2(cmd->file);
		exit(exec_build(cmd, env));
	}
	else
		exit(g_ret);
}

void	not_child_built(t_list *cmd, t_env *env)
{
	if (g_ret != 130 && cmd->cmds && !(ft_strcmp(cmd->cmds[0], "echo") \
	|| (ft_strcmp(cmd->cmds[0], "export") && !cmd->cmds[1]) || \
	(ft_strcmp(cmd->cmds[0], "env"))))
	{
		if (cmd->file)
			ft_dup_fd2(cmd->file);
		g_ret = exec_build(cmd, env);
	}
}

int	exec_build_not_pipe(t_list	*cmd, t_env *env)
{
	pid_t		pid;
	int			status;

	if (cmd->file)
	{
		restart_t_list_file(&(cmd->file));
		if (cmd->error == 0 && ft_redirection(cmd->file, env, 0) == -1)
			cmd->error = 3;
	}
	pid = fork();
	inhibit_signals(pid);
	cmd->pid = pid;
	cmd->is_fork = 1;
	if (pid == 0)
		child_pid_built(cmd, env);
	if (pid != 0)
		not_child_built(cmd, env);
	close_pipes(cmd, 0);
	waitpid(cmd->pid, &status, 0);
	if (WIFEXITED(status))
		g_ret = WEXITSTATUS(status);
	handle_signals();
	return (g_ret);
}
