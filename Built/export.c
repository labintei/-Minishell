/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:35:58 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:54:17 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_find_env(t_list_env	**env, char	*var)
{
	int			g;

	g = 0;
	list_start_env(env);
	while ((*env) && g == 0)
	{
		if ((*env) && (*env)->var && ft_strcmp((*env)->var, var))
			return (1);
		if ((*env) && (*env)->next)
			(*env) = (*env)->next;
		else
			g = 1;
	}
	return (0);
}

void	change_value(t_list_env **env, char *var, char *val)
{
	if ((*env))
	{
		if ((*env) && (*env)->var && ft_strcmp((*env)->var, var))
		{
			if ((*env)->val)
				free((*env)->val);
			ft_strcpy(&((*env)->val), val);
		}
	}
}

int	ft_find_concat(char *assign)
{
	int		i;

	while (assign && assign[i])
	{
		if (assign[i] == '=')
		{
			if (i > 0 && assign[i - 1] && assign[i - 1] == '+')
				return (1);
			return (0);
		}
		i++;
	}
	return (0);
}

void	concat_value(t_list_env **env, char *var, char *val)
{
	char		*temp;
	int			i;
	int			j;

	if ((*env))
	{
		if ((*env) && (*env)->var && ft_strcmp((*env)->var, var))
		{
			ft_strcpy(&temp, (*env)->val);
			if ((*env)->val)
				free((*env)->val);
			(*env)->val = malloc(sizeof(char) * (ft_strlen(temp) + \
			ft_strlen(val) + 1));
			i = -1;
			while (temp && temp[(++i)])
				(*env)->val[i] = temp[i];
			j = -1;
			while (val && val[(++j)])
				(*env)->val[i + j] = val[j];
			(*env)->val[i + j] = '\0';
			if (temp)
				free(temp);
		}
	}
}

int	export_build(t_list *cmds, t_env *env)
{
	int		i;

	i = 0;
	while (cmds->cmds && cmds->cmds[0] && cmds->cmds[(++i)])
	{
		if (ft_find('=', cmds->cmds[i]) && ft_find_concat(cmds->cmds[i]))
			make_concat(env, cmds->cmds[i]);
		else if (ft_find('=', cmds->cmds[i]))
			export_simple(cmds->cmds[i], env);
	}
	if (cmds->cmds[0] && !cmds->cmds[1])
	{
		list_start_env(&env->env);
		list_sort((env->env));
	}
	return (0);
}
