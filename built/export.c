/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:39:11 by labintei          #+#    #+#             */
/*   Updated: 2021/08/30 15:40:42 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			export_build(t_list *cmds, t_env *env)
{
	int		i;
	char	*var;
	char	*val;

	i = 1;
	if(cmds->cmds && cmds->cmds[0] && cmds->cmds[i])
	{
		if(ft_find("=",cmds->cmds[i]))
		{
			var = ft_strdup_char(env->cmds[i], '=');
			val = ft_strdup_char_after(env->cmds[i], "=");
			if(ft_find_env(env, l->var))

			else
			{
				
				add_env()

		}
	}
	else
	{
		// FAIRE LA FONCTION pour export sans arguemens;
	}
	return(0);
}
