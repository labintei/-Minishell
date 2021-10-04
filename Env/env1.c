/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 22:12:29 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 16:09:08 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_arg(t_list_env	*list, char *var, char *val)
{
	if (var)
	{
		ft_strcpy(&(list->var), var);
		if (val)
			ft_strcpy(&(list->val), val);
		else
			list->val = NULL;
	}
	return (1);
}

int	view_list_env(t_list_env	**l)
{
	t_list_env		*c;

	list_start_env(l);
	c = (*l);
	printf("\n");
	while (c)
	{
		if (c && !ft_strcmp(c->var, "?") && c->val)
		{
			printf("%s=", c->var);
			printf("%s\n", c->val);
		}
		c = c->next;
	}
	printf("\n");
	return (0);
}

int	add_list_env(t_list_env	**list, char	*var, char *val, int i)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
	new->previous = NULL;
	new->next = NULL;
	new->var = NULL;
	new->val = NULL;
	if (i != 0)
	{
		(*list)->next = new;
		new->previous = *list;
	}
	(*list) = new;
	if (!(add_arg(new, var, val)))
		return (0);
	return (1);
}

void	stock_env(t_env *env, char **envp)
{
	int		i;
	char	*var;
	char	*val;
	char	*s;

	i = 0;
	var = NULL;
	val = NULL;
	while (envp && envp[i])
	{
		var = ft_strdup_char(envp[i], '=');
		val = ft_strdup_char_after(envp[i], '=');
		if (var && ft_strcmp_1(var, "_") != 0)
			add_list_env(&(env->env), var, val, i);
		if (var)
			free(var);
		if (val)
			free(val);
		i++;
	}
	s = ft_itoa(g_ret);
	add_list_env(&(env->env), "?", s, i);
	if (s)
		free(s);
}

int	ft_strlen_env(t_list_env **list, char *var)
{
	t_list_env	*l;

	list_start_env(list);
	l = (*list);
	while (l)
	{
		if (l->var && ft_strcmp(l->var, var))
			return (ft_strlen(l->val));
		l = l->next;
	}
	return (0);
}
