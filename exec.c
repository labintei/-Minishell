/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 20:15:32 by labintei          #+#    #+#             */
/*   Updated: 2021/09/02 13:23:26 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_build(char *s)
{
	if(s && *s)
	{
		if(ft_strcmp(s, "echo") || ft_strcmp(s, "cd") || ft_strcmp(s, "pwd") \
		|| ft_strcmp(s, "unset") || ft_strcmp(s, "export") || \
		ft_strcmp(s, "exit"))
			return(1);
	}
	return(0);
}

int		exec_build(t_list	*a, t_env *env, bool fork)
{
//	return(exit_build(env));
	if(ft_strcmp(a->cmds[0], "cd"))
		return(cd(env->cmds, env->env, fork));
	if(ft_strcmp(a->cmds[0], "echo"))
		return(echo_build(env->cmds));
	if(ft_strcmp(a->cmds[0], "unset"))
		return(unset(env->cmds, env));
	if(ft_strcmp(a->cmds[0], "env"))
		return(export_build(env->cmds, env));
	if(ft_strcmp(a->cmds[0], "pwd"))
		return(pwd());
	if(ft_strcmp(a->cmds[0], "env"))
		return(0);
	if(ft_strcmp(a->cmds[0], "exit"))
		return(exit_build(env));
	else
		return(1);
}

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

int			exec_cmd(t_list *cmd, t_env *env)
{
	pid_t		pid;
	int			ret;
	int			status;
	int			pipe_open;

	ret  = 1;
	pipe_open = 0;
	if(cmd->type == '|' || (cmd->previous && cmd->previous->type == '|'))
	{
		pipe_open = 1;
		if(pipe(cmd->pipe))
			printf("\nERREUR EXIT FATAL\n");
	}
	pid = fork();
	if(pid < 0)
		printf("\nERREUR EXIT FATAL\n");
	else if(pid == 0)
	{
		if(cmd->type == '|' && dup2(cmd->pipe[1], 1) < 0)
			printf("\nERREUR EXIT FATAL\n");
		if(cmd->previous && cmd->previous->type == '|' && dup2(cmd->previous->pipe[0], 0) < 0)
			printf("\nERREUR EXIT FATAL\n");
		if(cmd && cmd->cmds && cmd->cmds[0])
			ret = exec_other(cmd, env);
		exit(ret);
	}
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
	return(ret);
}

/* besoin de savoir si c est forked ou pas */
void		exec_cmds(t_env *env)
{
	t_list	*c;
	bool	fork;
//	int	status;
//	int	ret;

	fork = false;
	list_cmds_restart(&(env->cmds));
//	printf("\nAAAAAAA\n");
	c = env->cmds;
	while(env->cmds)
	{
		c = env->cmds;
		if(c && c->cmds && c->cmds[0] && is_build(c->cmds[0]))
			exec_build(c, env, fork);
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



