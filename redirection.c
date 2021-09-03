/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 19:23:59 by labintei          #+#    #+#             */
/*   Updated: 2021/09/03 18:59:09 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_redirection(t_env *env)
{
	(void)env;
}

/*
void		redir_output_append()
{
	close(1);
	open("doc", O_RDWR | O_CREAT | O_APPEND, 0644);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}


void		redir_output_simple()
{
	close(1);
	open("doc", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}

void		redir_input_simple()
{
	close(0);
	open("doc", O_RDONLY);
	if(fork() == 0)
		execution(pathcmds, cmds, convert_env);
}*/
