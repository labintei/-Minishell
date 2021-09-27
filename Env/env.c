/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:39:08 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 22:17:23 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	ft_putstr_fd_err(void)
{
	ft_putstr_fd("Failure to allocate memory\n", 2);
	return (1);
}

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
			exit(ft_putstr_fd_err());
		return (0);
	}
	next = sub_add_arg_2(key, value);
	if (!next)
		exit(ft_putstr_fd_err());
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

void	list_start_env(t_list_env **list)
{
	while ((*list) && ((*list)->previous))
		*list = (*list)->previous;
}
