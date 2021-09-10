/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 15:19:10 by labintei          #+#    #+#             */
/*   Updated: 2021/09/10 17:06:06 by labintei         ###   ########.fr       */
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

void	skip_case_1(char *line, int *j, int *word)
{
	skip_space(line, j);
	if (line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
	{
		(*word)++;
		while (line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
			(*j)++;
	}
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

void	is_in_word(char *l, int *j, int *redir, int *pipe)
{
	char	c;

	while (l[(*j)] && l[(*j)] != ' ' && (*pipe) == 0 && (*redir) == 0)
	{
		if (is_quotes(l[(*j)], l, (*j)))
		{
			c = l[(*j)];
			(*j)++;
			while (l[(*j)] && l[(*j)] != c)
				(*j)++;
			(*j)++;
		}
		else
		{
			is_in_word_not_quotes(l, j, redir, pipe);
		}
	}
}

int	count_word(char *line, int *i)
{
	int	j;
	int	word;
	int	pipe;
	int	redir;

	word = 0;
	j = (*i);
	pipe = 0;
	skip_case_1(line, &j, &word);
	while (line && line[j] && pipe == 0)
	{
		spe_case(line, &j, &pipe, &redir);
		if (line[j] && line[j] != ' ' && redir == 0 && pipe == 0)
		{
			word++;
			is_in_word(line, &j, &redir, &pipe);
		}
	}
	return (word);
}
