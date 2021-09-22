/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 15:33:18 by labintei          #+#    #+#             */
/*   Updated: 2021/09/22 17:18:21 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


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
	exit(2);
}

/*
void		file_error(int n, t_env *env)
{
	
		

}

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

