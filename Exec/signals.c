/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 16:00:53 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 19:05:57 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

// GROSSO MODO handle_signals correspond a set_up_signals || inhibit_signals a 
// inhiber les signaux
// Les signaux doivent continuer a marcher dans les pipes 
// Exemple 
// cat | ls
// ^D
// (arret du process)

//
// SIG_IGN    ignore le signal
// SIG_DFL    Reset le signal
//

void	ctrl_backslash(int signal)
{
	(void)signal;
	ft_putstr_fd("\b\b", 0);
	ft_putstr_fd("  \b\b", 0);
}

void	ctrl_c(int signal)
{
	(void)signal;
	ft_putstr_fd("\b\b", 0);
	ft_putstr_fd("  \b\b", 0);
	write(1, "\n", 1);
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
	if (id == 0)
	{
		if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
			return (0);
		if (signal(SIGINT, SIG_DFL) == SIG_ERR)
			return (0);
	}
	else
	{
		if (signal(SIGQUIT, ignore_signals) == SIG_ERR)
			return (0);
		if (signal(SIGINT, ignore_signals) == SIG_ERR)
			return (0);
	}
	return (true);
}

int	handle_signals(void)
{
	if (signal(SIGQUIT, ctrl_backslash) == SIG_ERR)
		return (0);
	if (signal(SIGINT, ctrl_c) == SIG_ERR)
		return (0);
	return (1);
}
