/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 15:33:18 by labintei          #+#    #+#             */
/*   Updated: 2021/10/01 15:09:37 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	error_unexpected(int n, t_env *env)
{
	if (n == 1)
	{
		ft_putstr_fd("Error unexpected token : ", 2);
		if (env->none_ex == 'n')
			ft_putstr_fd("newline", 2);
		else if (env->none_ex == 'L')
			ft_putstr_fd("<<", 2);
		else if (env->none_ex == 'R')
			ft_putstr_fd(">>", 2);
		else
			write(2, &(env->none_ex), 1);
		write(2, "\n", 1);
		return (0);
	}
	return (1);
}

void	fatal_error(int n, t_env *env)
{
	(void)env;
	if (n == 1)
		ft_putstr_fd("\nFailed to allocate memory\n", 2);
	if (n == 2)
		ft_putstr_fd("\nEnvironement is empty\n", 2);
}

void	exit_fatal(int n, t_env *env)
{
	(void)n;
	(void)env;
	return ;
}

void	error_exec(int n, t_env *env)
{
	if (n == 1)
		ft_putstr_fd("\nFailed to Pipe\n", 2);
	if (n == 2)
	{
		ft_putstr_fd("\nFailed to Fork Process\n", 2);
		env->last_ret = 1;
	}
	if (n == 3)
		ft_putstr_fd("\nFailed to Dup fd\n", 2);
}
