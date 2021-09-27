/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 18:02:04 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:03:53 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	list_cmds_restart(t_list	**cmds)
{
	while ((*cmds) && (*cmds)->previous)
		(*cmds) = (*cmds)->previous;
}

int	ft_strcmp_cd(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

char	*ft_itoa(int n)
{
	char		*s;
	int			size;
	long int	i;

	size = 0;
	i = (long int)n;
	while (i && (++size))
		i /= 10;
	if (n == 0)
		size = 1;
	s = malloc(sizeof(char) * (size + 1));
	s[size] = '\0';
	if (n == 0)
		s[(--size)] = '0';
	else
	{
		while (n)
		{
			s[(--size)] = (n % 10) + '0';
			n /= 10;
		}
	}
	return (s);
}

int	ft_strlen(char	*s)
{
	int		i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}
