/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 19:28:15 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 19:50:07 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_only_var(char *line, int i)
{
	int	j;

	j = i;
	j++;
	while (line[j] && !(is_redir_pipe_space(line[j])))
	{
		if (line[j] == '\'' || line[j] == '\"')
			return (0);
		j++;
	}
	return (1);
}

void	parse_word_heredoc_0(char *line, int *i, t_env *env)
{
	int		quotes;
	int		j;
	char	c;

	quotes = 0;
	j = 0;
	while (line[(*i) + j] && !(is_redir_pipe_space(line[(*i)])))
	{
		if (line[(*i) + j] && is_quotes(line[(*i) + j], line, ((*i) + j)))
		{
			env->cmds->file->is_quotes = 1;
			c = line[(*i) + j];
			j++;
			quotes++;
			while (line[(*i) + j] && line[(*i) + j] != c)
				j++;
			j++;
			quotes++;
		}
		else
			j++;
	}
	env->cmds->file->path = malloc(sizeof(char) * ((j - quotes) + 1));
	if (!(env->cmds->file->path))
		exit_fatal(1, env);
}

void	parse_word_heredoc(char *line, int *i, t_env *env, int *count)
{
	char	c;

	parse_word_heredoc_0(line, i, env);
	(*count) = 0;
	while (line[(*i)] && !(is_redir_pipe_space(line[(*i)])))
	{
		if (line[(*i)] && (is_quotes(line[(*i)], line, (*i))))
		{
			c = line[(*i)];
			(*i)++;
			while (line[(*i)] && line[(*i)] != c)
			{
				env->cmds->file->path[(*count)] = line[(*i)];
				(*count)++;
				(*i)++;
			}
			(*i)++;
		}
		else
		{
			env->cmds->file->path[(*count)] = line[(*i)];
			(*count)++;
			(*i)++;
		}
	}
}
