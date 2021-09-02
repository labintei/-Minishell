/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 20:20:33 by labintei          #+#    #+#             */
/*   Updated: 2021/09/01 18:27:17 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		find_exec_path(char **path, t_env *env)
{
	struct		stat	si;
	int					i;
	char				*temp;
	int					j;
	int					h;

	i = -1;
	while(env->split_path && env->split_path[(++i)])
	{
		temp = NULL;
		temp = malloc(sizeof(char) * (ft_strlen(env->split_path[i]) + ft_strlen((*path)) + 1));
		j = -1;
		while(env->split_path[i] && env->split_path[i][(++j)])
			temp[j] = env->split_path[i][j];
		h = -1;
		while((*path) && (*path)[(++h)])
		{
			temp[j] = (*path)[h];
			j++;
		}
		temp[j] = '\0';
		j = open(temp, O_RDONLY);
		if(fstat(j, &si) != -1)
		{
			if((*path))
				free((*path));
			ft_strcpy(path, temp);
			if(temp)
				free(temp);
			return ;
		}
		if(temp)
			free(temp);
	}
}

void		find_path(t_list_env **env, char *s, char **val)
{
	t_list_env		*c;
	int				a;

	a = 0;
	list_start_env(env);
	c  = (*env);
	while(c)
	{
		if(c->var && ft_strcmp(c->var, s))
		{
			(*val) = malloc(sizeof(char) * (ft_strlen(c->val) + 1));
			while(c->val && c->val[a])
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

void		get_splitted_path(t_env *env)
{
	char	*s;
	char	*val;
	int		i[3];

	ft_strcpy(&s, "PATH");
//	s = "PATH";
	find_path(&(env->env), s, &val);
//	printf("\nPATH : %s\n", val);
	i[0] = 1;
	i[1] = -1;
	while(val && val[(++i[1])])
	{
		if(val && val[(i[1])] == ':')
			i[0]++;
	}
	env->split_path = malloc(sizeof(char*) * (i[0] + 1));
	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	while(val && val[(i[0])])
	{
		if(val && val[(i[0])] && val[(i[0])] != ':')
		{
			i[1] = 0;
			while(val[(i[0] + i[1])] && val[(i[0] + i[1])] != ':')
				(i[1])++;
			env->split_path[(i[2])] = malloc(sizeof(char) * (i[1] + 2));
			i[0]--;
			i[1] = 0;
			while(val[(++i[0])] && val[(i[0])] != ':')
			{
//``:				printf("\n%c\n", val[(i[0])]);
				env->split_path[(i[2])][(i[1])] = val[(i[0])];
				(i[1])++;
			}
			env->split_path[(i[2])][(i[1])] = '/';
			env->split_path[(i[2])][(i[1] + 1)] = '\0';
			(i[2])++;
		}
		if(val && val[(i[0])] && val[(i[0])] == ':')
			(i[0])++;
	}
	env->split_path[(i[2])] = NULL;
	if(val)
		free(val);
//	view_tab(env->split_path);
	if(s)
		free(s);
}
