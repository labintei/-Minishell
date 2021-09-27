/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 21:38:24 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:47:01 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	make_concat(t_env *env, char *s)
{
	int		i;
	char	*var;
	char	*val;

	i = 0;
	while (s && s[i] && s[i] != '=')
		i++;
	var = malloc(sizeof(char) * (i));
	if (!var)
		exit_fatal(1, env);
	i = -1;
	while (s && s[(++i)] && s[i + 1] != '=')
		var[i] = s[i];
	var[i] = '\0';
	val = ft_strdup_char_after(s, '=');
	if (var && is_only_alpha_num(var) && ft_find_env(&(env->env), var))
		concat_value(&(env->env), var, val);
	else if (var && is_only_alpha_num(var))
		add_list_env(&(env->env), var, val, 0);
	if (var)
		free(var);
	if (val)
		free(val);
}

void	export_simple(char	*s, t_env *env)
{
	char	*var;
	char	*val;

	var = ft_strdup_char(s, '=');
	val = ft_strdup_char_after(s, '=');
	if (var && is_only_alpha_num(var) && ft_find_env(&(env->env), var))
		change_value(&(env->env), var, val);
	else if (var && is_only_alpha_num(var))
		add_list_env(&(env->env), var, val, 4);
	if (var)
		free(var);
	if (val)
		free(val);
}

void	view_list_env_export(t_list_env	**l)
{
	t_list_env		*c;

	list_start_env(l);
	c = (*l);
	while (c)
	{
		if (c->var && ma_strcmp(c->var, "?") != 0)
		{
			printf("\ndeclare -x ");
			printf("%s=", c->var);
			printf("%s", c->val);
		}
		c = c->next;
	}
	printf("\n");
}

void	list_sort_0(t_list_env **sort)
{
	list_start_env(sort);
	view_list_env_export(sort);
	clear_list_env(sort);
}

void	list_sort(t_list_env *env)
{
	t_list_env	*sort;
	t_list_env	*c;
	int			i;

	c = env;
	if (!env)
		return ;
	add_list_env(&(sort), c->var, c->val, 0);
	c = env->next;
	while (c)
	{
		i = ma_strcmp(c->var, sort->var);
		while (c && sort && sort->next && i > 0)
		{
			sort = sort->next;
			i = ma_strcmp(c->var, sort->var);
		}
		if (i < 0)
			add_before(&(sort), c->var, c->val);
		else
			add_after(&(sort), c->var, c->val);
		list_start_env(&sort);
		c = c->next;
	}
	list_sort_0(&(sort));
}
