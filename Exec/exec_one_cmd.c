/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 14:08:11 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 15:53:33 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		child_not_built(t_list *cmd, t_env *env)
{
	if (cmd->file)
		ft_dup_fd2(cmd->file);
	if (cmd->error != 3 && g_ret != 130)
		return(exec_other(cmd, env));
	else
		return(g_ret);
}

int	exec_not_build_not_pipe(t_list	*cmd, t_env *env)
{
	pid_t		pid;
	int			status;

	status = 0;
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
		exit(child_not_built(cmd, env));
	if(pid != 0)
	{
		waitpid(cmd->pid, &status, 0);
		if (WIFEXITED(status))
			g_ret = WEXITSTATUS(status);
		close_pipes(cmd, 0);
		waitpid(cmd->pid, &status, 0);
	}
	handle_signals();
	return (g_ret);
}

int	exec_exit(t_list	*cmd, t_env *env)
{
	if (cmd->file)
	{
		restart_t_list_file(&(cmd->file));
		if (cmd->error == 0 && ft_redirection(cmd->file, env, 1) == -1)
			cmd->error = 3;
	}
	if (cmd->file)
		ft_dup_fd2(cmd->file);
	if (g_ret != 130)
		g_ret = exec_build(cmd, env);
	close_pipes(cmd, 0);
	handle_signals();
	return (g_ret);
}

int	exec_one_cmd(t_list *cmd, t_env *env)
{
	if (!env->error && !cmd->error && \
	cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]) && \
	!ft_strcmp(cmd->cmds[0], "exit"))
		g_ret = exec_build_not_pipe(cmd, env);
	else if (!env->error && !cmd->error && \
	cmd->cmds && cmd->cmds[0] && is_builtin(cmd->cmds[0]) && \
	ft_strcmp(cmd->cmds[0], "exit"))
		g_ret = exec_exit(cmd, env);
	else if (!env->error && !cmd->error)
		g_ret = exec_not_build_not_pipe(cmd, env);
	return (g_ret);
}
