/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:26:15 by labintei          #+#    #+#             */
/*   Updated: 2021/10/02 15:42:16 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	count_word(char *line, int *i, t_env *env)
{
	int	j;
	int	word;
	int	pipe;
	int	redir;

	(void)env;
	word = 0;
	j = (*i);
	pipe = 0;
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

void	add_cmds(t_list **cmds)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return ;
	new->type = '0';
	new->next = NULL;
	new->cmds = NULL;
	new->pipe[0] = 0;
	new->pipe[1] = 1;
	new->is_piped = 0;
	new->pipe_open = 0;
	new->is_fork = 0;
	new->error = 0;
	if ((*cmds))
	{
		(*cmds)->next = new;
		new->previous = (*cmds);
		(*cmds) = (*cmds)->next;
	}
	else
	{
		new->previous = NULL;
		(*cmds) = new;
	}
}
