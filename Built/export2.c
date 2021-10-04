/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 21:52:23 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:53:11 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_before(t_list_env **list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
	if (!new)
		return ;
	new->previous = (*list)->previous;
	new->next = (*list);
	new->var = NULL;
	new->val = NULL;
	add_arg(new, var, val);
	if ((*list)->previous)
		(*list)->previous->next = new;
	(*list)->previous = new;
}

int	is_only_alpha_num(char	*var)
{
	int		i;

	i = 0;
	if (var && is_alphanum(var[i]) && (var[i] >= '0' && var[i] <= '9'))
		return (0);
	while (var && var[i])
	{
		if (var[i] && !(is_alphanum(var[i])))
			return (0);
		i++;
	}
	return (1);
}

void	add_after(t_list_env **list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
	if (!new)
		return ;
	new->previous = (*list);
	new->next = (*list)->next;
	(*list)->next = new;
	new->var = NULL;
	new->val = NULL;
	add_arg(new, var, val);
}
