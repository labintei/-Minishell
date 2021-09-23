/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:42:04 by labintei          #+#    #+#             */
/*   Updated: 2021/09/23 16:04:33 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//voir les retours
int		find_exec_path(char **path, t_env *env)
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
			//revoir retours
			return (-1);
		}
		if(temp)
			free(temp);
	}
	ft_putstr_fd((*path), 2);
	ft_putstr_fd(": command not found\n", 2);
	return (0);
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

	if(env->env && ft_find_env(&(env->env), "PATH"))
	{
		ft_strcpy(&s, "PATH");
		find_path(&(env->env), s, &val);
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
	else
	{
		//if(env->split_path)
		//	clear_tab(&(env->split_path));
		env->split_path = NULL;
	}
}
