/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:42:04 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 12:04:40 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_path(t_list_env **env, char *s, char **val)
{
	t_list_env		*c;
	int				a;

	a = 0;
	list_start_env(env);
	c = (*env);
	while (c)
	{
		if (c->var && ft_strcmp(c->var, s))
		{
			(*val) = malloc(sizeof(char) * (ft_strlen(c->val) + 1));
			while (c->val && c->val[a])
			{
				(*val)[a] = (c->val)[a];
				a++;
			}
			(*val)[a] = '\0';
			return ;
		}
		c = c->next;
	}
}

void	init_int_3(int (*i)[3])
{
	(*i)[0] = 0;
	(*i)[1] = 0;
	(*i)[2] = 0;
}

void	count_number_path(t_env *env, char **val, char **s, int (*i)[3])
{
	ft_strcpy(s, "PATH");
	find_path(&(env->env), (*s), val);
	(*i)[0] = 1;
	(*i)[1] = -1;
	while ((*val) && (*val)[(++((*i)[1]))])
	{
		if ((*val) && (*val)[((*i)[1])] == ':')
			((*i)[0])++;
	}
	env->split_path = malloc(sizeof(char *) * (((*i)[0]) + 1));
	init_int_3(i);
}

void	get_splitted_path_bis(char *val, int (*i)[3], t_env *env)
{
	(*i)[1] = 0;
	while (val[((*i)[0] + (*i)[1])] && val[((*i)[0]) + ((*i)[1])] != ':')
		((*i)[1])++;
	env->split_path[((*i)[2])] = malloc(sizeof(char) * (((*i)[1]) + 2));
	((*i)[0])--;
	((*i)[1]) = 0;
	while (val[(++((*i)[0]))] && val[((*i)[0])] != ':')
	{
		env->split_path[((*i)[2])][((*i)[1])] = val[((*i)[0])];
		((*i)[1])++;
	}
	env->split_path[((*i)[2])][((*i)[1])] = '/';
	env->split_path[((*i)[2])][((*i)[1] + 1)] = '\0';
	((*i)[2])++;
}

void	get_splitted_path(t_env *env)
{
	char	*s;
	char	*val;
	int		i[3];

	if (env->env && ft_find_env(&(env->env), "PATH"))
	{
		count_number_path(env, &val, &s, &i);
		while (val && val[(i[0])])
		{
			if (val && val[(i[0])] && val[(i[0])] != ':')
				get_splitted_path_bis(val, &i, env);
			if (val && val[(i[0])] && val[(i[0])] == ':')
				(i[0])++;
		}
		env->split_path[(i[2])] = NULL;
		if (val)
			free(val);
		if (s)
			free(s);
	}
	else
		env->split_path = NULL;
}
