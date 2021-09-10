/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 15:29:43 by labintei          #+#    #+#             */
/*   Updated: 2021/09/10 17:15:07 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_quotes_char(char *line, int *j, int *count, t_env *env)
{
	char	c;

	c = line[(*j)];
	(*j)++;
	while (line[(*j)] && line[(*j)] != c)
	{
		if (c == '\"' && line[(*j)] == '$')
			(*count) += find_var_and_strlen(line, j, env);
		else
		{
			(*j)++;
			(*count)++;
		}
	}
	(*j)++;
}

void	not_quotes_char(char *line, int *j, t_env *env, int (*i)[2])
{
	while (line[(*j)] && (!(is_quotes(line[(*j)], line, (*j)))) && \
	line[(*j)] != '|' && line[(*j)] != ' ' && line[(*j)] != '<' && \
	line[(*j)] != '>')
	{
		if (line[(*j)] && line[(*j)] == '$')
			((*i)[0]) += find_var_and_strlen(line, j, env);
		else
		{
			((*i)[0])++;
			(*j)++;
		}
	}
	if (line[(*j)] && line[(*j)] == '|')
		(*i)[1] = 1;
	if (line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
		(*i)[1] = 1;
}

int		count_char(char *line, int h, t_env *env)
{
	int		i[2];
	int		j;

	j = h;
	i[0] = 0;
	i[1] = 0;
	while (line && line[j])
	{
		skip_space(line, &j);
		if (line[j] && line[j] != '|' && line[j] != ' ')
		{
			while (line[j] && line[j] != ' ' && i[1] == 0)
			{
				if (is_quotes(line[j], line, j))
					is_quotes_char(line, &j, &(i[0]), env);
				else
					not_quotes_char(line, &j, env, &i);
			}
			return (i[0]);
		}
		else
			return (0);
	}
	return (0);
}
