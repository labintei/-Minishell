/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 18:12:18 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 18:42:28 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ajout_cmds(t_env *env, char *line, int *i)
{
	add_cmds(&(env->cmds));
	if (!(env->cmds->file))
		return ;
	env->cmds->cmds = NULL;
	env->cmds->file = NULL;
	if (((count_word(line, i, env) + 1) - nb_redir(line, (*i))) > 0)
		env->cmds->cmds = malloc(sizeof(char *) * \
		(count_word(line, i, env) + 1 - nb_redir(line, (*i))));
}

void	is_pipe(t_env *env, char *line)
{
	env->cmds->type = '|';
	if (env->empty == 1 && env->error == 0)
	{
		env->error = 1;
		env->none_ex = (line[(env->i)]);
	}
	if (env->is_cmds && env->cmds && env->cmds->cmds)
		env->cmds->cmds[env->word] = NULL;
	env->is_cmds = 1;
	(env->i)++;
	env->word = 0;
	ajout_cmds(env, line, &(env->i));
	env->empty = 1;
}

int	is_redir(char *line, int *i, t_env *env)
{
	char	c;

	c = line[(*i)];
	(*i)++;
	if (line[(*i)] && line[(*i) - 1] && line[(*i)] == line[((*i) - 1)])
	{
		c = 'L';
		if (line[(*i)] == '>')
			c = 'R';
		(*i)++;
	}
	if ((env->last_type == 'r') && env->error == 0)
	{
		env->error = 1;
		env->none_ex = c;
	}
	env->last_type = 'r';
	skip_space(line, i);
	if (line[(*i)] && (line[(*i)] == '<' || line[(*i)] == '>'))
		return (2);
	add_list_file(&(env->cmds->file), count_char(line, (*i), env), c);
	return (0);
}

void	is_quotes_cmds(char *line, int *count, t_env *env)
{
	char	c;

	c = line[(env->i)];
	(env->i)++;
	while (line[(env->i)] && line[(env->i)] != c)
	{
		if (c == '\"' && line[(env->i)] == '$')
			find_var_and_strlen_cmds(line, env, count);
		else
		{
			if (env->is_cmds)
				env->cmds->cmds[(env->word)][(*count)] = line[(env->i)];
			else
				env->cmds->file->path[(*count)] = line[(env->i)];
			(env->i)++;
			(*count)++;
		}
	}
	(env->i)++;
}

void	ft_dup_env(t_list_env **env, char *var, char **s)
{
	int			a;
	t_list_env	*l;

	l = (*env);
	while (l)
	{
		if (l->var && ft_strcmp(l->var, var))
		{
			(*s) = malloc(sizeof(char) * (ft_strlen(l->val) + 1));
			a = -1;
			while (l->val && l->val[(++a)])
				(*s)[a] = l->val[a];
			(*s)[a] = '\0';
			return ;
		}
		l = l->next;
	}
}
