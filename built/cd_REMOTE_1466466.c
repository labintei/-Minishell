/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:01:00 by labintei          #+#    #+#             */
/*   Updated: 2021/09/02 18:52:29 by malatini         ###   ########.fr       */
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

int	ft_strcmp_cd(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

/* Returns the value of the corresponding key env */
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
/* revoir la valeur de fork */
char	*find_path_1(t_list *cmds, t_list_env *env, bool fork)
{
	int		length;
	char	*path;

	length = cmds_length(cmds);
	fork = 1;
	//printf("la commande est : %s\n", cmds->cmds[1]);
	//printf("length = %i\n", length);
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

/* Adds a env_list elem when it does not exists already*/
t_list_env	*sub_add_arg_2(char *key, char *value)
{
	t_list_env	*next;

	next = (t_list_env *)malloc(sizeof(t_list_env));
	if (!next)
	{
		ft_putstr_fd("Failure to allocate memory\n", 2);
		exit (EXIT_FAILURE);
	}
	next->var = ft_strdup(key);
	next->val = ft_strdup(value);
	next->next = NULL;
	if (!next->var || ! next->val)
	{
		ft_putstr_fd("Failure to allocate memory\n", 2);
		exit (EXIT_FAILURE);
	}
	return (next);
}

/* Edit or add a env list elem */
int	add_arg_2(t_list_env *env, char *key, char *value)
{
	t_list_env	*next;
	t_list_env	*current;

	current = env;
	if (ft_strcmp_cd(env->var, "") == 0 && ft_strcmp(env->val, "") == 0)
	{
		free(env->var);
		free(env->val);
		env->var = ft_strdup(key);
		env->val = ft_strdup(value);
		if (!env->var || !env->val)
		{
			ft_putstr_fd("Failure to allocate memory\n", 2);
			exit (EXIT_FAILURE);
		}
		return (0);
	}
	next = sub_add_arg_2(key, value);
	if (!next)
	{
		ft_putstr_fd("Failure to allocate memory\n", 2);
		exit (EXIT_FAILURE);
	}
	while (current->next)
		current = current->next;
	current->next = next;
	return (0);
}

int	env_manager(char *key, char *value, t_list_env *env)
{
	t_list_env	*elem;

	elem = env;
	while (elem)
	{
		if (ft_strcmp_cd(elem->var, key) == 0)
		{
			if (!value)
				return (0);
			free(elem->val);
			elem->val = ft_strdup(value);
			if (!elem->val && value)
			{
				ft_putstr_fd("Failure to allocate memory\n", 2);
				exit (EXIT_FAILURE);
			}
			return (0);
		}
		elem = elem->next;
	}
	if ((add_arg_2(env, key, value)) != 0)
		return (-1);
	return (0);
}

/* executes the cd commands */
int	cd(t_list *cmds, t_list_env *env, bool fork)
{
	int		ret;
	char	*previous;
	char	*current;

	current = find_path_1(cmds, env, fork);
//	printf("current is %s\n", current);
	if (current == NULL)
		return (CD_ERROR);
	previous = getcwd(NULL, 0);
//	printf("previous is %s\n", previous);
	ret = chdir(current);
	if (ret)
		cd_errors(current, errno, fork);
	else
	{
		if (previous)
			env_manager("OLDPWD", previous, env);
		//printf("previous is now %s\n", previous);
		current = getcwd(NULL, 0);
		if (current == NULL)
			return (CD_ERROR);
		env_manager("PWD", current, env);
		//printf("current is %s\n", current);
		free(current);
	}
	free(previous);
	return (ret);
}
