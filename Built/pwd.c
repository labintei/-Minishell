/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:36:27 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 22:00:10 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(t_env *env)
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
