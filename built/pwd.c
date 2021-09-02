/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:01:19 by labintei          #+#    #+#             */
/*   Updated: 2021/09/02 13:23:55 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			pwd(t_env *env)
{
	char	*path;

	path = find_value(env->env, "PWD");
	if (!path)
	{
		ft_putstr_fd("Environment var not found\n", 2);
		return (1);
	}
	printf("%s\n", path);
	return (0);
}
