/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 14:11:36 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 20:40:14 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../minishell.h"
/*
int	count_redir(char *line, int j)
{
	int		f[4];

	f[1] = 0;
	f[0] = j;
	f[3] = 0;
	while (line[f[0]] && f[3] == 0)
	{
		if (line[f[0]] && f[3] == 0 && (line[f[0]] == '\'' || line[f[0]] == '\"') && ft_second(line[f[0]], line, f[0]))
		{
			f[2] = (int)line[f[0]];
			(f[0])++;
			while (line[f[0]] && line[f[0]] != f[2])
				(f[0])++;
			if (line[f[0]] == f[2])
				(f[0])++;
		}
		else
		{
			while (line[f[0]] && f[3] == 0 && !((line[f[0]] == '\'' || line[f[0]] == '\"') && ft_second(line[f[0]], line, f[0])))
			{
				if (line[f[0]] && line[f[0]] == '|')
					f[3] = 1;
				else if (line[f[0]] && (line[f[0]] == '<' || line[f[0]] == '>'))
				{
					f[2] = (int)line[f[0]];
					(f[0])++;
					if (line[f[0]] == f[2])
						(f[0])++;
					(f[1])++;
				}
				(f[0])++;
			}
		}
	}
	return (f[1]);
}*/
