/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 18:01:02 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 21:02:31 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	list_env_len(t_list_env *env)
{
	int			i;
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

int	is_builtin(char *s)
{
	if (s)
	{
		if (!ma_strcmp(s, "echo") || !ma_strcmp(s, "cd") || \
		!ma_strcmp(s, "pwd") || !ma_strcmp(s, "unset") || \
		!ma_strcmp(s, "export") || !ma_strcmp(s, "exit") || \
		!ma_strcmp(s, "env"))
			return (true);
	}
	return (false);
}
