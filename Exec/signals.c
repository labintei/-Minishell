/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 16:00:53 by labintei          #+#    #+#             */
/*   Updated: 2021/10/01 16:07:37 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ctrl_backslash(int signal)
{
	(void)signal;
	g_ret = 131;
	ft_putstr_fd("\b\b", 0);
	ft_putstr_fd("  \b\b", 0);
}

void	ctrl_c(int signal)
{
	(void)signal;
	g_ret = 130;
	ft_putstr_fd("\b\b", 0);
	ft_putstr_fd("  \b\b", 0);
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ignore_signals(int signal)
{
	(void)signal;
	if (signal == SIGINT)
		write(1, "\n", 1);
}

int	inhibit_signals(int id)
{
	if (id == 0)
	{
		if ((signal(SIGQUIT, SIG_IGN)) == SIG_ERR)
			return (0);
		if ((signal(SIGINT, SIG_DFL)) == SIG_ERR)
			return (0);
	}
	else
	{
		if ((signal(SIGQUIT, SIG_IGN)) == SIG_ERR)
			return (0);
		if ((signal(SIGINT, SIG_IGN)) == SIG_ERR)
			return (0);
	}
	return (1);
}

int	handle_signals(void)
{
	if ((signal(SIGQUIT, SIG_IGN)) == SIG_ERR)
		return (0);
	if ((signal(SIGINT, ctrl_c)) == SIG_ERR)
		return (0);
	return (1);
}
