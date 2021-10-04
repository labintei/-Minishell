/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 23:53:19 by labintei          #+#    #+#             */
/*   Updated: 2021/10/03 23:54:18 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc(sizeof(char) * n + 1);
	if (!res)
		return (NULL);
	while (str[i] && i < n)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_get_val(char *var, t_env *env)
{
	t_list_env	*tmp;

	tmp = env->env;
	while (tmp && strcmp(tmp->var, var) != 0)
		tmp = tmp->next;
	free(var);
	if (tmp && tmp->val)
		return (tmp->val);
	return (NULL);
}

int	ft_search_var(char *line, t_list_env *tmp, int *i, char *res)
{
	int		j;

	j = (*i) + 1;
	while (is_alphanum(line[j]) > 0)
		j++;
	res = ft_strndup(&line[(*i) + 1], j - (*i) - 1);
	while (tmp && strcmp(tmp->var, res) != 0)
		tmp = tmp->next;
	if (res)
		free(res);
	(*i) = j;
	if (tmp && tmp->val)
		return (ft_strlen(tmp->val));
	else
		return (0);
}
