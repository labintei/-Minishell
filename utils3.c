/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:30:45 by malatini          #+#    #+#             */
/*   Updated: 2021/09/09 18:31:25 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strnewcat(char *first, char *second)
{
	int 	i;
	int 	j;
	char	*new;
	int		len_s;
	
	len_s = ft_strlen(second);
	new = (char *)malloc(ft_strlen(first) + len_s);
	if (!new)
		return (NULL);
	i = 0;
	while (first && first[i])
	{
		new[i] = first[i];
		i++;
	}
	j = 0;
	while (j < len_s)
	{
		new[i + j] = second[j];
		j++;
	}
	new[i + j] = '\0';
	if (first)
		free(first);
	return (new);
}