/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 23:47:42 by labintei          #+#    #+#             */
/*   Updated: 2021/10/03 23:51:22 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_without(t_env *env, char *var)
{
	if (var && is_only_alpha_num(var) && ft_find_env(&(env->env), var))
		return ;
	else if (var && is_only_alpha_num(var))
		add_list_env(&(env->env), var, NULL, 4);
}

int	ft_error_export(char *s)
{
	if (s && !((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')))
		return (1);
	return (0);
}

void	ft_error_export_message(char *s)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("':not a valid identifier", 2);
}
