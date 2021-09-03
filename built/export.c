/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:39:11 by labintei          #+#    #+#             */
/*   Updated: 2021/09/03 18:22:32 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void		add_list_env_between(t_list_env	**list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
//	if(!new)
//		return ;
	new->previous = (*list);
	new->next = (*list)->next;
	(*list)->next = new;
//	if((*list)->next)
//		(*list)->next->previous = new;
	new->var = NULL;
	new->val = NULL;
//	(*list) = new;
	add_arg(new, var, val);
}

void		add_before(t_list_env **list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));

	new->previous = (*list)->previous;
	new->next = (*list);
	(*list)->previous = new;
	new->var = NULL;
	new->val = NULL;
	add_arg(new, var, val);
}

void		add_after(t_list_env **list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));

	new->previous = (*list);
	new->next = (*list)->next;
	(*list)->next = new;
	new->var = NULL;
	new->val = NULL;
	add_arg(new, var, val);
}

int			sort_str(char *s, char *sbis)
{
	int		i;

	i = 0;
	if(!sbis && !s)
		return(0);
	if(!s && sbis && sbis[0])
		return(-sbis[0]);
	if(!sbis && s && s[0])
		return(s[0]);
	while(s && sbis && s[i] && sbis[i] && s[i] == sbis[i])
		i++;
	return(s[i] - sbis[i]);
}

//void		clear_list_env(t_list_env	**env)


void		view_list_env_export(t_list_env	**l)
{
	t_list_env		*c;

	list_start_env(l);
	c = (*l);
	printf("\nVIEW_ENV\n");
	while(c)
	{
		printf("\ndeclare -x ");
		if(c->var)
			printf("%s=", c->var);
		if(c->val)
			printf("%s", c->val);
		c = c->next;
	}
	printf("\n");
}


void		list_sort(t_list_env *env)
{
	t_list_env	*sort;
	t_list_env	*temp;
	t_list_env	*c;
	int			i;
	int			g = 0;

	printf("\nOn est dans export\n");
	c = env;
	if(!env)
		return ;
	add_list_env(&(sort), c->var, c->val, 0);
	c = env->next;
	while(c)
	{
		temp = c->next;
		i = 0;
		while(c && sort && sort->next && (i = strcmp(c->var, sort->var)) > 0)
		{
			printf("\n%d\n", g);
			sort = sort->next;
		}
		if(g == 0 && i <= 0)
			add_before(&(sort), c->var, c->val);
		else
		add_list_env_between(&(sort), c->var, c->val);
		list_start_env(&sort);
		c = temp;
		g++;
	}
	list_start_env(&(sort));
	view_list_env_export(&(sort));
	clear_list_env(&(sort));
}

void		env_sans_arg(t_list_env		**env)
{
	list_start_env(env);
	list_sort((*env));
}

void		list_env_end(t_list_env		**env)
{
	list_start_env(env);
	while((*env) && (*env)->next)
		(*env) = (*env)->next;
}

int			ft_find_env(t_list_env **env, char *var)
{
	int			g;

	g = 0;
	list_start_env(env);
	while((*env) && g == 0)
	{
		if((*env) && (*env)->var && ft_strcmp((*env)->var, var))
			return(1);
		if((*env) && (*env)->next)
			(*env) = (*env)->next;
		else
			g = 1;
	}
	return(0);
}

void		change_value(t_list_env **env, char *var, char *val)
{
	if((*env))
	{
		if((*env) && (*env)->var && ft_strcmp((*env)->var, var))
		{
			if((*env)->val)
				free((*env)->val);
			ft_strcpy(&((*env)->val), val);
		}
	}
}

int			export_build(t_list *cmds, t_env *env)
{
	int		i;
	char	*var;
	char	*val;
	char	*temp;
	char	*to;
	int		g;

	g = 4;
	i = 1;
	while(cmds->cmds && cmds->cmds[0] && cmds->cmds[i])
	{
		if(ft_find('=',cmds->cmds[i]))
		{
			var = ft_strdup_char(cmds->cmds[i], '=');
			val = ft_strdup_char_after(cmds->cmds[i], '=');
			ft_strcpy(&temp, var);
			ft_strcpy(&to, val);
			if(var && ft_find_env(&(env->env), var))
			{
				change_value(&(env->env), var, val);
			}	
			else
			{
				add_list_env(&(env->env), temp, to, g);
			}
			if(var)
				free(var);
			if(val)
				free(val);
			if(temp)
				free(temp);
			if(to)
				free(to);
		}
		i++;
	}
	if(cmds->cmds[0] && !cmds->cmds[1])
	{
		env_sans_arg(&(env->env));
	}
	return(0);
}
