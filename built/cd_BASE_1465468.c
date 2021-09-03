/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:01:00 by labintei          #+#    #+#             */
/*   Updated: 2021/08/30 16:46:30 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstr_fd(char const *s, int fd)
{
	int		i;

	i = 0;
	if (s)
		while (s[i])
			write(fd, &s[i++], 1);
}

/* returns the number of arguments from commands' list */
int	cmds_length(t_list *cmds)
{
	int	i;

	i = 0;
	while (cmds->cmds[i])
		i++;
	return (i);
}

/* Returns the value of the corresponding key env */
char	*find_value(t_list_env *env, char *key)
{
	t_list_env	*elem;

	elem = env;
	while (elem)
	{
		if (ft_strcmp(key, elem->var) == 0)
			return (elem->val);
		elem = elem->next;
	}
	return (NULL);
}

/* Prints current cd errors */
void	cd_errors(char *path, int err, bool fork)
{
	if (!fork)
		return ;
	if (err == ENOTDIR)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
	}
	else if (err == ENOENT)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

/* Finds the target path for cd depending on the arguments */
char	*find_path(t_list *cmds, t_list_env *env, bool fork)
{
	int		length;
	char	*path;

	length = cmds_length(cmds);
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

int	env_manager(char *key, char *value, t_list_env *env)
{
	t_list_env	*elem;

	elem = env;
	while (elem)
	{
		if (ft_strcmp(elem->var, key) == 0)
		{
			if (!value)
				return (0);
			free(elem->val);
			elem->val = NULL;
			elem->val = ft_strdup(value);
			if (!elem->val)
			{
				ft_putstr_fd("Failure to allocate memory\n", 2);
				exit (EXIT_FAILURE);
			}
			return (0);
		}
		elem = elem->next;
	}
	add_arg(env, key, value);
	return (0);
}

/* executes the cd commands */
int	cd(t_list *cmds, t_list_env *env, bool fork)
{
	int		ret;
	char	*previous;
	char	*current;

	current = find_path(cmds, env, fork);
	if (current == NULL)
		return (CD_ERROR);
	previous = getcwd(NULL, 0);
	ret = chdir(current);
	if (!ret)
		cd_errors(current, errno, fork);
	else
	{
		if (previous)
			env_manager("OLDPWD", previous, env);
		current = getcwd(NULL, 0);
		if (current == NULL)
			return (CD_ERROR);
		env_manager("PWD", current, env);
		free(current);
	}
	free(previous);
	return (ret);
}