/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 15:34:11 by malatini          #+#    #+#             */
/*   Updated: 2021/09/09 16:21:01 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* retourne la longueur de la liste chainee de var d'env */
int		list_env_len(t_list_env *env)
{
	int 		i;
	t_list_env	*elem;

	i = 0;
	elem = env;
	while (elem)
	{
		elem = elem->next;
		i++;
	}
	return (i);
}

/* A mettre dans un fichier utils */
void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	unsigned char	*buf_dest;
	unsigned char	*buf_src;

	buf_dest = (unsigned char *)dest;
	buf_src = (unsigned char *)src;
	if (dest == src)
		return (dest);
	while (size--)
	{
		*(buf_dest++) = *(buf_src++);
	}
	return (dest);
}

//J'ai rajoute les ! devant ft_strcmp par rapport a Lauranne 
int			is_builtin(char *s)
{
	if (s)
	{
		if (!ft_strcmp(s, "echo") || !ft_strcmp(s, "cd") || !ft_strcmp(s, "pwd") \
		|| !ft_strcmp(s, "unset") || !ft_strcmp(s, "export") 
		|| !ft_strcmp(s, "exit") || !ft_strcmp(s, "env"))
			return (true);
	}
	return (false);
}

bool	is_piped(t_list *elem)
{
	if (elem->type == '|')
		return (true);
	else if (elem->previous && elem->previous->type == '|')
		return (true);
	return (false);
}


bool	is_redirected(t_list *elem)
{
	if (elem)
	{
		if (elem->file->redir == '<' || elem->file->redir == '>'
		|| elem->file->redir == 'L' || elem->file->redir == 'R')
			return (true);
	}
	return (false);
}