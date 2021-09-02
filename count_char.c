/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 15:29:43 by labintei          #+#    #+#             */
/*   Updated: 2021/09/02 13:16:01 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		is_quotes_char(char *line, int *j, int *count,  t_env *env)
{
	char	c;

	c = line[(*j)];
	(*j)++;
	while(line[(*j)] && line[(*j)] != c)
	{
		if(c == '\"' && line[(*j)] == '$')
			(*count) += find_var_and_strlen(line, j, env);
		else
		{
			(*j)++;
			(*count)++;
		}
	}
	(*j)++;
}

void		not_quotes_char(char *line, int *j, t_env *env, int *count, int *out)
{
	while(line[(*j)] && (!(is_quotes(line[(*j)], line, (*j)))) && \
	line[(*j)] != '|' && line[(*j)] != ' ' && line[(*j)] != '<' && \
	line[(*j)] != '>')
	{
		if(line[(*j)] && line[(*j)] == '$')
			(*count) += find_var_and_strlen(line, j, env);
		else
		{
			(*count)++;
			(*j)++;
		}
	}
	if(line[(*j)] && line[(*j)] == '|')
		*out = 1;
	if(line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
		*out = 1;

}

int			count_char(char *line, int	i, t_env *env)
{
	int		j;
	int		out;
	int		count;

	j = i;
	count = 0;
	out = 0;
	while(line && line[j])
	{
		skip_space(line, &j);
		if(line[j] && line[j] != '|' && line[j] != ' ')
		{
			while(line[j] && line[j] != ' ' && out == 0)
			{
				if(is_quotes(line[j], line, j))
					is_quotes_char(line, &j, &count, env);
				else
					not_quotes_char(line, &j, env, &count, &out);
			}
			return(count);
		}
		else
			return(0);
	}
	return (0);
}


