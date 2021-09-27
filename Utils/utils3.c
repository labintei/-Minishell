/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 18:01:30 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:03:17 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ma_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strnewcat(char *first, char *second)
{
	int		i;
	int		j;
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

int	cmds_length(t_list *cmds)
{
	int	i;

	i = 0;
	while (cmds->cmds[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char const *s, int fd)
{
	int		i;

	i = 0;
	if (s)
		while (s[i])
			write(fd, &s[i++], 1);
}
