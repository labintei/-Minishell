/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:52:39 by malatini          #+#    #+#             */
/*   Updated: 2021/09/09 17:20:13 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Va servir pour le char ** a envoyer a execve */
char	*flatten_env_var(t_list_env *env, t_env *mem)
{
	char 	*ret;
	int		i;
	int		total_len;

	ret = NULL;
	(void)mem;
	total_len = ft_strlen(env->var) + 1 + ft_strlen(env->val) + 1;
	ret = (char *)malloc(sizeof(char *) * total_len);
	//fonction de gestion des mauvais malloc
	i = ft_strlen(env->var);
	ret[i] = '=';
	ret[total_len - 1] = '\0';
	ft_memcpy(ret, env->var, ft_strlen(env->var));
	ft_memcpy(&(ret[i]), env->val, ft_strlen(env->val));
	return (ret);
}

/* Va servir pour le char ** a envoyer a execve */
char	**ft_env_string_tab(t_env *env)
{
	int 		i;
	t_list_env 	*elem;
	char		**ret;
	int			length;

	i = 0;
	length = list_env_len(env->env);
	ret = (char **)malloc(sizeof(char *) * (length + 1));
	//fonction de free exit proprement en cas d appel systeme rate
	ret[length] = NULL;
	elem = env->env;
	while (elem && i < length)
	{
		ret[i] = flatten_env_var(elem, env);
		//fonction de gestion des erreurs malloc
		i++;
		elem = elem->next;
	}
	return (ret);
}