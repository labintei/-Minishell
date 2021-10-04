/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_new_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 10:41:33 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 11:00:57 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_o(char ***test, t_env *env, t_list *c)
{
	ft_convert_env(&(env->env), test);
	g_ret = execve(c->cmds[0], c->cmds, (*test));
	if (test)
		clear_tab(test);
	return (g_ret);
}

int	erreur_o(t_list	*c)
{
	ft_putstr_fd(c->cmds[0], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	g_ret = 1;
	return (1);
}

int	exec_other(t_list	*c, t_env *env)
{
	char			**test;
	int				j;
	struct stat		si;

	g_ret = 0;
	if (c->pid == 0)
	{
		if (c->cmds && c->cmds[0] && c->cmds[0][0] && \
		c->cmds[0][0] == '.' && c->cmds[0][1] && c->cmds[0][1] == '/')
		{
			j = open(c->cmds[0], O_RDONLY);
			if (fstat(j, &si) == -1)
				exit(erreur_o(c));
			exec_o(&(test), env, c);
		}
		else if (c->cmds && c->cmds[0])
		{
			j = open(c->cmds[0], O_RDONLY);
			if (fstat(j, &si) == -1)
				find_exec_path(&(c->cmds[0]), env);
			exec_o(&(test), env, c);
		}
	}
	exit(g_ret);
	return (g_ret);
}

int	wait_exec_cmds(t_list	*c)
{
	int			ret;
	int			status;
	t_list		*cmds;

	cmds = c;
	ret = 0;
	while (cmds)
	{
		if (cmds->is_fork)
		{
			waitpid(cmds->pid, &status, 0);
			if (WIFEXITED(status) && g_ret != 130)
				g_ret = WEXITSTATUS(status);
		}
		cmds = cmds->next;
	}
	handle_signals();
	return (ret);
}

void	close_fd(t_list_file	**file)
{
	t_list_file		*temp;

	restart_t_list_file(file);
	temp = (*file);
	while (temp)
	{
		if (temp->redir != 'L' && temp->fd > 0)
			close(temp->fd);
		temp = temp->next;
	}
	temp = (*file);
	while (temp)
	{
		if (temp->redir == 'L')
		{
			close(temp->pipe_fd[0]);
			close(temp->pipe_fd[1]);
		}
		temp = temp->next;
	}
}
