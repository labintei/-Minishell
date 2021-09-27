/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:34:39 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:08:38 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_value(t_list_env *env, char *key)
{
	t_list_env	*elem;

	elem = env;
	while (elem)
	{
		if (ft_strcmp_cd(key, elem->var) == 0)
		{
			return (elem->val);
		}
		elem = elem->next;
	}
	return (NULL);
}

int	cd_errors(t_list *cmds, int err, bool fork, char *path)
{
	(void)fork;
	if (cmds->cmds && cmds->cmds[0] && cmds->cmds[1] && cmds->cmds[2])
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(": trop d'arguments\n", 2);
		return (1);
	}
	else if (err == ENOTDIR)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
		return (1);
	}
	else if (err == ENOENT)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

char	*find_path_1(t_list *cmds, t_list_env *env, bool fork)
{
	int		length;
	char	*path;

	length = cmds_length(cmds);
	fork = 1;
	if (length < 2)
	{
		path = find_value(env, "HOME");
		if (!path && fork)
			ft_putstr_fd("cd : HOME not set\n", 2);
	}
	else
	{	
		if (cmds->cmds[1][0] == '-' && cmds->cmds[1][1] == '\0')
		{
			path = find_value(env, "OLDPWD");
			if (!path && fork)
				ft_putstr_fd("cd : OLDPWD not set\n", 2);
			else if (fork)
				printf("%s\n", path);
		}
		else
			path = cmds->cmds[1];
	}
	return (path);
}

int	cd(t_list *cmds, t_list_env *env, bool fork)
{
	int		ret;
	char	*previous;
	char	*current;
	int		i;

	i = 0;
	current = find_path_1(cmds, env, fork);
	if (current == NULL)
		return (CD_ERROR);
	previous = getcwd(NULL, 0);
	ret = chdir(current);
	if (cd_errors(cmds, errno, fork, current))
		i = 1;
	if (previous && i == 0)
		env_manager("OLDPWD", previous, env);
	current = getcwd(NULL, 0);
	if (current == NULL)
		return (CD_ERROR);
	if (i == 0)
		env_manager("PWD", current, env);
	if (current)
		free(current);
	if (previous)
		free(previous);
	return (ret);
}
