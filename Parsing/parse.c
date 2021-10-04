/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:57:26 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 19:51:38 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_word_cmds_0(char *line, int *i, t_env *env)
{
	if (!env->cmds->cmds && (count_word(line, i, env) - \
	nb_redir(line, (*i))) > 0)
		env->cmds->cmds = malloc(sizeof(char *) * \
		(count_word(line, i, env) - nb_redir(line, (*i)) + 1));
	if (env->cmds->cmds)
		env->cmds->cmds[(env->word)] = malloc(sizeof(char) * \
		(count_char(line, (*i), env) + 1));
}

void	is_word_cmds_1(char	*line, int	*count, int	*i, t_env *env)
{
	int		out;

	out = 0;
	if (!env->is_cmds && env->last_type == 'r' && env->cmds->file \
	&& env->cmds->file->redir == 'L')
		parse_word_heredoc(line, i, env, count);
	else
	{
		while (line[(*i)] && line[(*i)] != ' ' && out == 0)
		{
			if (is_quotes(line[(*i)], line, (*i)))
				is_quotes_cmds(line, count, env);
			else
				not_quotes_cmds(line, env, count, &out);
		}
	}
}

void	is_word_cmds(char *line, int *i, t_env *env)
{
	int	count;

	count = 0;
	if ((env->is_cmds))
		is_word_cmds_0(line, i, env);
	if (!env->is_cmds && env->cmds->error == 0 && env->last_type == 'r' && \
	(env->cmds->file && env->cmds->file->redir != 'L') && \
	line[(*i)] && line[(*i)] == '$' && is_only_var(line, (*i)) \
	&& count_char(line, (*i), env) == 0)
		save_ambigous(&(env->cmds->file->ambigous), line, (*i));
	if (line[(*i)] && line[(*i)] != '|' && line[(*i)] != ' ')
		is_word_cmds_1(line, &count, i, env);
	if ((env->is_cmds))
		env->cmds->cmds[(env->word)][count] = '\0';
	else
		env->cmds->file->path[count] = '\0';
	if ((env->is_cmds))
		(env->word)++;
	if ((env->is_cmds) == 0)
		(env->is_cmds) = 1;
}

int	next_is_word(char *line, int i)
{
	while (line && line[i])
	{
		if (line && line[i] == ' ')
			i++;
		else if (line && (line[i] == '<' || line[i] == '>' || line[i] == '|'))
			return (0);
		else
			return (1);
	}
	return (0);
}

void	init_parse_line(t_env *env, char *line)
{
	env->cmds = NULL;
	env->error = 0;
	env->empty = 1;
	env->none_ex = 0;
	env->i = 0;
	env->word = 0;
	env->is_cmds = 1;
	env->last_type = 0;
	ajout_cmds(env, line, &(env->i));
}
