/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 16:00:53 by labintei          #+#    #+#             */
/*   Updated: 2021/09/05 18:40:00 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>

/* Attention aux différences Mac/Linux
** Ce code n'est adapte que pour la VM ou les postes de l'ecole sous Linux
*/

/* handle the ctrl + \ signal by 
** Les \b\b permettent d'empecher les retours charriots pour "ne rien faire"
** Le control backslash correspond habituellement à un sigquit (quit program)
** Nous sommes obliges de mettre un argument pour la fonction signal. 
*/
void	ctrl_backslash(int signal)
{
	(void)signal;
	ft_putstr_fd("\b\b", 0);
	ft_putstr_fd("  \b\b", 0);
}

/* Les \b\b permettent d'empecher de reecrire par dessus le print
** Le control c correspond à un sigint (interrupt program)
** revoir le details des lignes, retester chaque signal
*/
void	ctrl_c(int signal)
{
	(void)signal;
	ft_putstr_fd("\b\b", 0);
	ft_putstr_fd("  \b\b", 0);
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	//faire un free mem ?
}

/* Parent process will not quit after a SIGINT signal */
void	ignore_signals(int signal)
{
	if (signal == SIGINT)
		write(1, "\n", 1);
}

/*
int	inhibit_signals(int id)
{
	if (id == 0)
	{
		signal(SIGQUIT, ctrl_backslash);
		signal(SIGINT, ctrl_c);
	}
	else
	{
		signal(SIGQUIT, ignore_signals);
		signal(SIGINT, ignore_signals);
	}
	return (true);
}

int	handle_signals(void)
{
	signal(SIGQUIT, ctrl_backslash);
	signal(SIGINT, ctrl_c);
	return (true);
}

*/


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

/* Handles signals as the subject is requesting it 
** The control D is working as expected without having to do anything.
*/
int	handle_signals(void)
{
	if (signal(SIGQUIT, ctrl_backslash) == SIG_ERR)
		return (0);
	if (signal(SIGINT, ctrl_c) == SIG_ERR)
		return (0);
	return (1);
}
