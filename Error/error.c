/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 15:33:18 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 16:05:04 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



void		ft_putstr_err(int err, char c, char *s)
{
	// ERREUR DE PARSING FATAL SI | avec que des espaces vides
	// ou sucession de < > sans arguments
	// ou egalement une redirection ou il n y a rien apres
	//
	ft_putstr_fd("minishell: erreur de syntaxe pres du symbole inattendu \" %c \"", 2);
	// redirectin invalide par exemple $FUEEHWW et la variable est non valide
	ft_putstr_fd("minishell: %s : redirection ambigue\n", s);
	// commande introuvable
	ft_putstr_fd("%s : commande introuvable", s);
	//


}

