/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 15:19:10 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 20:17:47 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redir_pipe_space(char c)
{
	if (c && (c == '<' || c == '>' || c == '|' || c == ' '))
		return (1);
	return (0);
}

void	skip_space(char *line, int *j)
{
	while (line && line[(*j)] && line[(*j)] == ' ')
		(*j)++;
}

int	is_quotes(char c, char *line, int i)
{
	if ((c == '\'' || c == '\"') && ft_second(c, line, i))
		return (1);
	return (0);
}

void	spe_case(char	*line, int *j, int *pipe, int *redir)
{
	(*redir) = 0;
	skip_space(line, j);
	if (line[(*j)] && line[(*j)] == '|')
		(*pipe) = 1;
	if (line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
	{
		while (line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
			(*j)++;
	}
}

void	is_in_word_not_quotes(char *l, int *j, int *redir, int *pipe)
{
	while (l[(*j)] && (!(is_quotes(l[(*j)], l, (*j)))) && \
	(!(is_redir_pipe_space(l[(*j)]))))
		(*j)++;
	if (l[(*j)] && l[(*j)] == '|')
		(*pipe) = 1;
	if (l[(*j)] && (l[(*j)] == '<' || l[(*j)] == '>'))
	{
		(*redir) = 1;
		while (l[(*j)] && (l[(*j)] == '<' || l[(*j)] == '>'))
			(*j)++;
	}
}
