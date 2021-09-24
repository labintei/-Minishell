/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 18:01:30 by labintei          #+#    #+#             */
/*   Updated: 2021/09/24 15:34:54 by labintei         ###   ########.fr       */
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


char		*ft_itoa(int n)
{
	char *s;
	int		size;
	long int	i;

	size = 0;
	i = (long int)n;
	while(i && (++size))
		i /= 10;
	if(n == 0)
		size = 1;
	s = malloc(sizeof(char) * (size + 1));
	s[size] = '\0';
	if(n == 0)
		s[(--size)] = '0';
	else
	{
		while(n)
		{
			s[(--size)] = (n % 10) + '0';
			n /= 10;
		}
	}
	return(s);
}


