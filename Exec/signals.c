/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 16:00:53 by labintei          #+#    #+#             */
/*   Updated: 2021/09/30 21:41:45 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// #include <stdint.h>

//void		(*)int	c;

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
//faire un free mem ?
}

void	ctrl_c_heredoc(int signal)
{
	(void)signal;
	g_ret = 130;
//	ft_putstr_fd("\b\b", 0);
//	ft_putstr_fd("  \b\b", 0);
//	write(1, "\n", 1);
//	rl_replace_line("", 0);
//	rl_on_new_line();
//	rl_redisplay();
//faire un free mem ?
}



void	ignore_signals(int signal)
{
	(void)signal;
	if (signal == SIGINT)
		write(1, "\n", 1);
}

int	inhibit_signals(int id)
{
	void	*i;

	i = 0;
	if (id == 0)
	{
		if ((i = (signal(SIGQUIT, SIG_DFL))) < 0)
			return(0);
		if((i = (signal(SIGINT, SIG_DFL))) < 0)
			return (0);
	}
	else
	{
		if ((i = (signal(SIGQUIT, SIG_IGN))) < 0)
			return(0);
		if((i = (signal(SIGINT, SIG_IGN))) < 0)
			return (0);
	}
	return (1);
}

int	handle_signals_bis(void)
{
	void	*i;

	i = 0;
	if ((i = (signal(SIGQUIT, SIG_DFL))) < 0)
		return (0);
	if ((i = (signal(SIGINT, ctrl_c_heredoc))) < 0)
		return (0);
	return (1);
}

int	handle_signals(void)
{
	void	*i;

	i = 0;
	if ((i = (signal(SIGQUIT, ctrl_backslash))) < 0)
		return (0);
	if ((i = (signal(SIGINT, ctrl_c))) < 0)
		return (0);
	return (1);
}
