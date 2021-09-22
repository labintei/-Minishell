/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:38:44 by labintei          #+#    #+#             */
/*   Updated: 2021/09/22 18:38:43 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void		unset_in_env(t_list_env **env, char *var)
{
	t_list_env	*previous;
	t_list_env	*next;

	if(*env)
	{
		if((*env)->var && ft_strcmp((*env)->var, var))
		{
			next = (*env)->next;
			previous = (*env)->previous;
			if((*env)->var)
				free((*env)->var);
			if((*env)->val)
				free((*env)->val);
			if((*env))
				free(*env);
			if(next)
				next->previous = previous;
			if(previous)
				previous->next = next;
			if(previous)
				(*env) = previous;
			else
				(*env) = next;
		}
	}
	list_start_env(env);
}


int			unset(t_list *cmds, t_env *env)
{
	int			i;

	i = 0;
	list_start_env(&(env->env));
	while(cmds->cmds && cmds->cmds[0] && cmds->cmds[i])
	{
		if(env->env && ft_find_env(&(env->env), cmds->cmds[i]))
			unset_in_env(&(env->env), cmds->cmds[i]);
		i++;
	}
	if(!(env->env))
		exit_fatal(2, env);
	return (0);
}
