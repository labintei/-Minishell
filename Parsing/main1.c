/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:55:05 by labintei          #+#    #+#             */
/*   Updated: 2021/10/01 15:18:07 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restart_t_list_file(t_list_file **file)
{
	while ((*file) && (*file)->previous)
		(*file) = (*file)->previous;
}

void	add_list_file(t_list_file **file, int i, char c)
{
	t_list_file		*new;

	new = malloc(sizeof(t_list_file));
	if (!new)
		return ;
	new->next = NULL;
	new->redir = c;
	new->is_quotes = 0;
	new->fd = -2;
	new->ambigous = NULL;
	if (c != 'L')
		new->path = malloc(sizeof(char) * (i + 1));
	else
		new->path = NULL;
	if (*file)
	{
		(*file)->next = new;
		new->previous = (*file);
		(*file) = (*file)->next;
	}
	else
	{
		new->previous = NULL;
		(*file) = new;
	}
}

int	find_var_and_strlen(char *line, int *j, t_env *env)
{
	char	*new;
	int		a;
	int		ret;

	a = 0;
	(*j)++;
	while (line && line[(*j) + a] && is_alphanum(line[(*j) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a = 0;
	while (line && line[(*j)] && is_alphanum(line[(*j)]))
	{
		new[a] = line[(*j)];
		a++;
		(*j)++;
	}
	new[a] = '\0';
	ret = ft_strlen_env(&(env->env), new);
	if (new)
		free(new);
	return (ret);
}

void	skip_quotes(char *line, int *j)
{
	char	c;

	if (line && line[(*j)] && is_quotes(line[(*j)], line, (*j)))
	{
		c = line[(*j)];
		(*j)++;
		while (line[(*j)] && line[(*j)] != c)
			(*j)++;
		(*j)++;
	}
}

int	nb_redir(char *line, int h)
{
	int		i[3];

	i[1] = 0;
	i[0] = h;
	while (line && line[i[0]])
	{
		if (line[i[0]] && is_quotes(line[i[0]], line, i[0]))
			skip_quotes(line, &(i[0]));
		else if (line[i[0]] && line[i[0]] == '|')
		{
			return (i[1]);
		}
		else if (line[i[0]] && ((line[i[0]] == '<') | (line[i[0]] == '>')))
		{
			i[2] = line[i[0]];
			(i[0])++;
			if (line[i[0]] && line[i[0]] == (char)i[2])
				(i[0])++;
			if (next_is_word(line, (i[0])))
				(i[1])++;
		}
		else
			(i[0])++;
	}
	return (i[1]);
}
