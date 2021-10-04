/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:54:22 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:04:07 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_strcpy(char **s, char *copy)
{
	int		i;

	i = -1;
	(*s) = malloc(sizeof(char) * (ft_strlen(copy) + 1));
	if (!(*s))
		return ;
	while (copy[(++i)])
		(*s)[i] = copy[i];
	(*s)[i] = '\0';
}

int	ft_strcmp(char *s, char *sbis)
{
	int		i;

	i = 0;
	if (!s || !sbis)
		return (0);
	while (s && sbis && s[i] && sbis[i] && s[i] == sbis[i])
		i++;
	if (s && sbis && s[i] == '\0' && sbis[i] == '\0')
		return (1);
	return (0);
}

int	ft_second(char c, char *s, int i)
{
	while (s && s[(++i)])
	{
		if (s[i] == c)
			return (1);
	}
	return (0);
}

int	is_alphanum(char c)
{
	if ((c >= 48 && c <= 57) || (c >= 97 && c <= 122) || \
	(c >= 65 && c <= 90) || (c == 95) || (c == 63))
		return (1);
	return (0);
}

char	ft_find(char c, char *s)
{
	int		i;

	i = -1;
	while (s && s[(++i)])
	{
		if (s[i] == c)
			return (c);
	}
	return (0);
}
