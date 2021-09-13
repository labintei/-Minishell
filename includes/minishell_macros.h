/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_macros.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 12:17:42 by malatini          #+#    #+#             */
/*   Updated: 2021/09/13 12:23:51 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_MACROS_H
# define MINISHELL_MACROS_H

#include "../minishell.h"

/*
** Exit Codes
*/
# define E_CATCHALL 1
# define E_MISUSE   2
# define E_CEXEC    126
# define E_NOTF     127
# define E_INVALID  128
//128+n ?
# define E_SIG      130
# define E_OUT      255

#endif
