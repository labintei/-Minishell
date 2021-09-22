/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:35:58 by labintei          #+#    #+#             */
/*   Updated: 2021/09/22 18:36:44 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		add_before(t_list_env **list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
	if(!new)
		return ;
	new->previous = (*list)->previous;
	new->next = (*list);
	new->var = NULL;
	new->val = NULL;
	add_arg(new, var, val);
	if((*list)->previous)
		(*list)->previous->next = new;
	(*list)->previous = new;
}

int			is_only_alpha_num(char	*var)
{
	int		i;

	i = 0;
	if(var && is_alphanum(var[i]) && (var[i] >= '0' && var[i] <= '9'))
		return(0);
	while(var && var[i])
	{
		if(var[i] && !(is_alphanum(var[i])))
			return(0);
		i++;
	}
	return(1);
}

void		add_after(t_list_env **list, char *var, char *val)
{
	t_list_env	*new;

	new = malloc(sizeof(t_list_env));
	if(!new)
		return ;
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

void		view_list_env_export(t_list_env	**l)
{
	t_list_env		*c;

	list_start_env(l);
	c = (*l);
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
	t_list_env	*c;
	int			i;
	int			g = 0;

	c = env;
	if(!env)
		return ;
	add_list_env(&(sort), c->var, c->val, 0);
	c = env->next;
	while(c)
	{
		i = 0;
		while(c && sort && sort->next && (i = ma_strcmp(c->var, sort->var)) > 0)
		{
			sort = sort->next;
		}
		if(i < 0)
			add_before(&(sort), c->var, c->val);
		else
			add_after(&(sort), c->var, c->val);
		list_start_env(&sort);
		c = c->next;
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

int			ft_find_concat(char *assign)
{
	int		i;

	while(assign && assign[i])
	{
		if(assign[i] == '=')
		{
			if(i > 0 && assign[i - 1] && assign[i - 1] == '+')
				return(1);
			return(0);
		}
		i++;
	}
	return(0);
}

void		concat_value(t_list_env **env, char *var, char *val)
{
	char		*temp;
	int			i;
	int			j;

	if((*env))
	{
		if((*env) && (*env)->var && ft_strcmp((*env)->var, var))
		{
			ft_strcpy(&temp, (*env)->val);
			if((*env)->val)
				free((*env)->val);
			(*env)->val = malloc(sizeof(char) * (ft_strlen(temp) + ft_strlen(val) + 1));
			i = 0;
			while(temp && temp[i])
			{
				(*env)->val[i] = temp[i];
				i++;
			}
			j = 0;
			while(val && val[j])
			{
				(*env)->val[i + j] = val[j];
				j++;
			}
			(*env)->val[i + j] = '\0';
			if(temp)
				free(temp);
		}
	}
}

void		make_concat(t_env *env, char *s)
{
	int		i;
	char	*var;
	char	*val;
	int		j;
	int		g;

	i = 0;
	g = 0;
	while(s && s[i] && s[i] != '=')
		i++;
	var = malloc(sizeof(char) * (i));
	if(!var)
		exit_fatal(1, env);
	i = 0;
	while(s && s[i] && s[i + 1] != '=')
	{
		var[i] = s[i];
		i++;
	}
	var[i] = '\0';
	if(s[i] == '+' && s[i + 1] == '=')
		i += 2;
	val = malloc(sizeof(char) * ((ft_strlen(s) - 1 - ft_strlen(var))));
	if(!val)
		exit_fatal(1, env);
	j = 0;
	while(s && s[i + j])
	{
		val[j] = s[i + j];
		j++;
	}
	val[j] = '\0';
	if(var && is_only_alpha_num(var) && ft_find_env(&(env->env), var))
		concat_value(&(env->env), var, val);
	else if(var && is_only_alpha_num(var))
		add_list_env(&(env->env), var, val, g);
	if(var)
		free(var);
	if(val)
		free(val);
}


int			export_build(t_list *cmds, t_env *env)
{
	int		i;
	char	*var;
	char	*val;
	int		g;

	g = 4;
	i = 1;
	while(cmds->cmds && cmds->cmds[0] && cmds->cmds[i])
	{
		if(ft_find('=', cmds->cmds[i]) && ft_find_concat(cmds->cmds[i]))
		{
			make_concat(env, cmds->cmds[i]);
		}
		else if(ft_find('=',cmds->cmds[i]))
		{
			var = ft_strdup_char(cmds->cmds[i], '=');
			val = ft_strdup_char_after(cmds->cmds[i], '=');
			if(var && is_only_alpha_num(var) && ft_find_env(&(env->env), var))
				change_value(&(env->env), var, val);
			else if(var && is_only_alpha_num(var))
				add_list_env(&(env->env), var, val, g);
			if(var)
				free(var);
			if(val)
				free(val);
		}
		i++;
	}
	if(cmds->cmds[0] && !cmds->cmds[1])
	{
		env_sans_arg(&(env->env));
	}
	return(0);
}
