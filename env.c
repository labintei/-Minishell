/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:54:03 by labintei          #+#    #+#             */
/*   Updated: 2021/09/06 13:31:42 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		list_start_env(t_list_env	**list)
{
	while((*list) && ((*list)->previous))
		*list = (*list)->previous;
}

int			ft_len_env(t_list_env **env)
{
	int		i;
	t_list_env	*c;

	i = 0;
	list_start_env(env);
	c = (*env);
	while(c)
	{
		i++;
		c = c->next;
	}
	return(i);
}

int			view_list_env(t_list_env	**l)
{
	t_list_env		*c;

	list_start_env(l);
	c = (*l);
	printf("\n");
//	printf("\nVIEW_ENV\n");
	while(c)
	{
		printf("%s=", c->var);
		printf("%s\n", c->val);
		c = c->next;
	}
	printf("\n");
	return(0);
}
void		add_arg(t_list_env	*list, char *var, char *val)
{
	ft_strcpy(&(list->var), var);
	ft_strcpy(&(list->val), val);
}


void		add_list_env(t_list_env	**list, char	*var, char *val, int i)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
//	if(!new)
//		return ;
	new->previous = NULL;
	new->next = NULL;
	new->var = NULL;
	new->val = NULL;
//	ft_strcpy(&(new->var), var);
//	ft_strcpy(&(new->val), val);
//	new->var = var;
//	new->val = val;
	if(i != 0)
	{
		(*list)->next = new;
		new->previous = *list;
	}
	(*list) = new;
	add_arg(new, var, val);
}

void		stock_env(t_env		*env, char **envp)
{
	int		i;
	char	*var;
	char	*val;

	i = 0;
	var = NULL;
	val = NULL;
	while(envp && envp[i])
	{
		var = ft_strdup_char(envp[i], '=');
		val = ft_strdup_char_after(envp[i], '=');
		add_list_env(&(env->env), var , val, i);
		if(var)
			free(var);
		if(val)
			free(val);
		i++;
	}
//	view_list_env(&(env->env));
}

int			ft_strlen_env(t_list_env **list, char *var)
{
	t_list_env	*l;

	list_start_env(list);
//	printf("\nVIEW-LIST\n");
//	view_list_env(list);
	l = (*list);
	while(l)
	{
		if(l->var && ft_strcmp(l->var, var))
			return(ft_strlen(l->val));
		l = l->next;
	}
	return(0);
}

// 
