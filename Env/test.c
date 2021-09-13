/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 21:29:55 by labintei          #+#    #+#             */
/*   Updated: 2021/09/13 21:32:55 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			count_char_expansion(char *s, t_env *env)
{
	int		i;

	i = 0;
	while(s && s[i])
	{
		if(s[i] != $)
			i++;
		else 


	}


}


void		ft_expansion(char line, char **temp)
{
	int		i

}
