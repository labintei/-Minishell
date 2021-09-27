/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 12:22:03 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 20:24:49 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strlen_char(char	*s, char c)
{
	int	i;

	i = -1;
	while (s && s[(++i)])
	{
		if (s[i] == c)
			return (i);
	}
	return (i);
}

char	*ft_strdup(char *src)
{
	int		i;
	char	*dest;

	if (!src)
		return (NULL);
	i = 0;
	while (src[i])
		i++;
	dest = malloc(sizeof(char) * (i + 1));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup_char(char *copy, char c)
{
	char	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(char) * (ft_strlen_char(copy, c) + 1));
	if (!new)
		return (NULL);
	while (copy && copy[i] && copy[i] != c)
	{
		new[i] = copy[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*ft_strdup_char_after(char *copy, char c)
{
	char	*new;
	int		i;
	int		j;

	j = 0;
	new = malloc(sizeof(char) * ((ft_strlen(copy) - \
	ft_strlen_char(copy, c)) + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (copy && copy[i] && copy[i] != c)
		i++;
	if (copy && copy[i] == c)
		i++;
	while (copy && copy[i])
	{	
		new[j] = copy[i];
		j++;
		i++;
	}
	new[j] = '\0';
	return (new);
}
