/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 14:42:10 by labintei          #+#    #+#             */
/*   Updated: 2021/09/16 14:44:24 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<limits.h>
# include	<stdlib.h>
# include	<stdio.h>
# include	<unistd.h>
# include	<readline/readline.h>
# include	<readline/history.h>
# include	<errno.h>
# include	<sys/wait.h>
# include	<sys/time.h>
# include	<sys/stat.h>
# include	<curses.h>
# include	<signal.h>
# include	<fcntl.h>
# include	<stdbool.h>
# include	<errno.h>

void	ft_putstr_fd(char const *s, int fd)
{
	int		i;

	i = 0;
	if (s)
		while (s[i])
			write(fd, &s[i++], 1);
}


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
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
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
