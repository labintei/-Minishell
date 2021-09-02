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

<<<<<<< HEAD
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
=======
/* a revoir Lauranne/Mahaut */
int			export_build(t_list *a, t_env *env)
{
    (void)a;
    (void)env;
    /*
	int		i;
	char	*val;
	char	*env_stock;

	i = 1;
	if(a->s && a->s[0] && a->s[1])
	{
		while(a->s[i])
		{
			if(a->s[i] && ft_find('=',a->s[i]))
			{
				val = NULL;
				env_stock = NULL;
				copy_jusqua(&val, a->s[i], '=');
				copy_apres(&env_stock, a->s[i], '=');
				if(val && !ft_find_env(val, env->env_s->first))
					ajout_env(val,env_stock, env->env_s);
				else if(val && ft_find_env(val, env->env_s->first))
					replace_env(val, env_stock, env->env_s); 
			}
			i++;
		}
	}
    */
	return(0);
}
>>>>>>> 0bfd98f3c85f47d77a9d5cbfba7b35758ae8861c
