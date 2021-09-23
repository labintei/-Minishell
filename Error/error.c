/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 15:33:18 by labintei          #+#    #+#             */
/*   Updated: 2021/09/23 16:36:25 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Grosso modo

int			error_unexpected(int n, t_env *env)
{
	if(n == 1)
	{
		ft_putstr_fd("Error unexpected token : ", 2);
		if(env->none_ex == 'n')
			ft_putstr_fd("newline", 2);
		else if(env->none_ex == 'L')
			ft_putstr_fd("<<", 2);
		else if(env->none_ex == 'R')
			ft_putstr_fd(">>", 2);
		else
			write(2, &(env->none_ex), 1);
		write(2, "\n", 1);
		return(0);
	}
	return(1);
}

void		fatal_error(int n, t_env *env)
{
	(void)env;
	if(n == 1)
		ft_putstr_fd("\nFailed to allocate memory\n", 2);
	if(n == 2)
		ft_putstr_fd("\nEnvironement is empty\n", 2);
}

void		exit_fatal(int n, t_env *env)
{
	(void)n;
	(void)env;
//	printf("\n%d\n", n);
//	exit(2);
	return ;
}

void		error_exec(int n, t_env *env)
{
	if(n == 1)
		ft_putstr_fd("\nFailed to Pipe\n", 2);
	if(n == 2)
	{
		ft_putstr_fd("\nFailed to Fork Process\n", 2);
		env->last_ret = 1;
	}
	if(n == 3)
	{
		ft_putstr_fd("\nFailed to Dup fd\n", 2);
	}
	if(n == 4)
	{
		
	}
}

/*
void		file_error(int n, t_env *env)
{
	
		

}

	Filename too long (superieur a 255 char) // Depend de l OS

	Syntax error near unexpexted token
	Unable to execute
	Failure in fork process
	Command not found
	Is a directory
	No such file or directory
	Syntax error
	Filename argument required
	Unknow error
	Faillure to allocate memory
	Permission denied
	Environnement var exists and overwrite is off
	Environnemnt var not found
	Environment is empty
	Invalid Expansion
}

void		ft_putstr_err(int err, char c, char *s)
{
	 ERREUR DE PARSING FATAL SI | avec que des espaces vides
	 ou sucession de < > sans arguments
	 ou egalement une redirection ou il n y a rien apres
	
	ft_putstr_fd("minishell: erreur de syntaxe pres du symbole inattendu \" %c \"", 2);
	ft_putstr_fd("minishell: %s : redirection ambigue\n", s);
	ft_putstr_fd("%s : commande introuvable", s);
	
*/

/*
Erreur 2
}
	Incapable d executer
	Fail dans le fork process
	Es un dossier
	Pas de tel fichier ou directory
	Erreur de syntax
	Filename argument required
}

Erreur Other
{
	Erreur inconnu
	Echec dans l allocation memoire
	Permission refuse
	Les variables d envirronements existent et overwrite est oof
	Variables d environnement non trouve
	L environnement est vide
	Expansion invalide
	sinon
		Erreur 2
}*/

