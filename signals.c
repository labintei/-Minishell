/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 16:00:53 by labintei          #+#    #+#             */
/*   Updated: 2021/09/02 14:05:26 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/* Should be called at the execution stage when a fork has been made
** The child process will handle signals as requested and the parent
** will ignore the signals during the child's execution. 
** The error return value is actually -1 and errno is set.  
*/
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

/* Handles signals as the subject is requesting it 
** The control D is working as expected without having to do anything.
*/
int	handle_signals(void)
{
	signal(SIGQUIT, ctrl_backslash);
	signal(SIGINT, ctrl_c);
	return (true);
}
