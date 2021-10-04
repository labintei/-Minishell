/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:48:50 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 20:05:37 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_line_0(t_env *env, char *l)
{
	if (l[(env->i)] && (l[(env->i)] == '>' || l[(env->i)] == '<'))
	{
		is_redir(l, &(env->i), env);
		env->is_cmds = 0;
	}
	else
	{
		env->empty = 0;
		is_word_cmds(l, &(env->i), env);
		env->last_type = 'w';
	}
}

void	parse_line(t_env *env, char *l)
{
	env->cmds = NULL;
	if (!l || l[0] == '\0' || is_only_space(l))
		return ;
	init_parse_line(env, l);
	while (l && l[(env->i)])
	{
		skip_space(l, &(env->i));
		if (l[(env->i)] && l[(env->i)] == '|')
			is_pipe(env, l);
		else if (l[(env->i)] && l[(env->i)] != '|' && l[(env->i)] != ' ')
			parse_line_0(env, l);
	}
	if (env->cmds && env->cmds->cmds)
		env->cmds->cmds[(env->word)] = NULL;
	if (env->last_type == 'r' && env->error == 0)
	{
		env->error = 1;
		env->none_ex = 'n';
	}
}

void	start_parse(t_env *env)
{
	while (true)
	{
		env->line = readline("\033[1;36muser@minishell$ \033[0m$ ");
		if (!env->line)
		{
			ft_putstr_fd("\b\b", 0);
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (env->line)
			parse_line(env, env->line);
		if (env->cmds && error_unexpected(env->error, env))
			exec_cmds(env);
		if (env->cmds)
			clear_cmds(&(env->cmds));
		add_history(env->line);
		if (!env->line)
			printf("\n");
		else
			free(env->line);
		if (env->split_path)
			clear_tab(&(env->split_path));
		get_splitted_path(env);
	}
}

void	init_env(t_env *env)
{
	env->cmds = NULL;
	env->env = NULL;
	env->split_path = NULL;
	env->empty = 1;
	env->error = 0;
	env->none_ex = 0;
	env->line = NULL;
}

int		g_ret;

int	main(int argc, char **argv, char **envp)
{
	t_env	env;

	(void)argv;
	g_ret = 0;
	if (argc == 1)
	{
		handle_signals();
		ascii_art();
		env.split_path = NULL;
		stock_env(&env, envp);
		get_splitted_path(&env);
		env.cmds = NULL;
		start_parse(&env);
	}
	clear_env(&env);
	return (g_ret);
}
