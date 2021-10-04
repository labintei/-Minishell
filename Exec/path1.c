/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:20:07 by labintei          #+#    #+#             */
/*   Updated: 2021/10/04 12:04:19 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_exec_path_bis(char	**temp, char *path, t_env *env, int (*g)[3])
{
	(*temp) = malloc(sizeof(char) * \
	(ft_strlen(env->split_path[((*g)[0])]) + ft_strlen((path)) + 1));
	(*g)[1] = -1;
	while (env->split_path[((*g)[0])] && \
	env->split_path[((*g)[0])][(++((*g)[1]))])
		(*temp)[(*g)[1]] = env->split_path[(*g)[0]][(*g)[1]];
	(*g)[3] = -1;
	while ((*path) && (path)[(++((*g)[3]))])
	{
		(*temp)[(*g)[1]] = (path)[((*g)[3])];
		((*g)[1])++;
	}
	(*temp)[((*g)[1])] = '\0';
	(*g)[1] = open((*temp), O_RDONLY);
}

int	find_exec_path(char **path, t_env *env)
{
	struct stat			si;
	char				*temp;
	int					g[3];

	g[0] = -1;
	while (env->split_path && env->split_path[(++(g[0]))])
	{
		temp = NULL;
		find_exec_path_bis(&temp, (*path), env, &g);
		if (fstat((g[1]), &si) != -1)
		{
			if ((*path))
				free((*path));
			ft_strcpy(path, temp);
			if (temp)
				free(temp);
			return (-1);
		}
		if (temp)
			free(temp);
	}
	ft_putstr_fd((*path), 2);
	ft_putstr_fd(": command not found\n", 2);
	return (0);
}
