/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 19:19:17 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 19:22:31 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_var_and_strlen_2(char *stock, t_env *env, int *count)
{
	int		a;

	a = 0;
	while (stock && stock[a])
	{
		if (env->is_cmds)
			env->cmds->cmds[(env->word)][(*count)] = stock[a];
		else
			env->cmds->file->path[(*count)] = stock[a];
		(*count)++;
		a++;
	}
}

void	find_var_and_strlen_cmds(char *line, t_env *env, int *count)
{
	char	*new;
	char	*stock;
	int		a;

	a = 0;
	(env->i)++;
	while (line && line[(env->i) + a] && is_alphanum(line[(env->i) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a = 0;
	while (line && line[(env->i)] && is_alphanum(line[(env->i)]))
	{
		new[a] = line[(env->i)];
		a++;
		(env->i)++;
	}
	new[a] = '\0';
	stock = NULL;
	ft_dup_env(&(env->env), new, &stock);
	find_var_and_strlen_2(stock, env, count);
	if (new)
		free(new);
	if (stock)
		free(stock);
}

int	is_only_space(char *line)
{
	int	i;

	i = 0;
	while (line && line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}

void	not_quotes_cmds(char *line, t_env *env, int *count, int *out)
{
	while (line[(env->i)] && (!(is_quotes(line[(env->i)], line, (env->i)))) && \
	line[(env->i)] != '|' && line[(env->i)] != ' ' && line[(env->i)] != '<' && \
	line[(env->i)] != '>')
	{
		if (line[(env->i)] && line[(env->i)] == '$')
			find_var_and_strlen_cmds(line, env, count);
		else
		{
			if (env->is_cmds == 1)
				env->cmds->cmds[(env->word)][(*count)] = line[(env->i)];
			else
				env->cmds->file->path[(*count)] = line[(env->i)];
			(*count)++;
			env->i++;
		}
	}
	if (line[(env->i)] && line[(env->i)] == '|')
		*out = 1;
	if (line[(env->i)] && (line[(env->i)] == '<' || line[(env->i)] == '>'))
		*out = 1;
}

int	save_ambigous(char **stock, char *line, int i)
{
	int		j;

	j = (i);
	while (line[j] && !(is_redir_pipe_space(line[j])))
		j++;
	(*stock) = malloc(sizeof(char) * ((j - (i)) + 1));
	if (!(*stock))
		return (0);
	j = 0;
	while (line[(i)] && !(is_redir_pipe_space(line[j])))
	{
		(*stock)[j] = line[(i)];
		j++;
		++(i);
	}
	(*stock)[j] = '\0';
	return (1);
}
