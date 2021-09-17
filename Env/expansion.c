/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:39:38 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 17:39:41 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* retourne la valeur d'une cle (variable d'environnement)
** Necessaire pour l'expansion.
*/
char	*get_value_from_key(char *str, int *i)
{
	char 	*buffer;
	int		j;

	buffer = (char *)malloc(sizeof(char));
	j = 1;
	if (!buffer)
	//fonction d erreur en cas de mauvais malloc
		return (NULL);
	buffer[0] = 0;
	if (str[j] == '?')
	{
		*i += 2;
		return (ft_strnewcat(buffer, "?"));
	}
	while (buffer[j])
	{
		if (!is_alphanum(buffer[j]))
		{
			*i += j;
			return (buffer);
		}
		buffer = ft_strnewcat(buffer, &str[j]);
	}
	*i += j;
	return (buffer);
}

/* A tester - fonction deja faite 
** Fonction principale qui gere l expansion
?*/
char *ft_expansion(char *str, t_env *env, int i, char *read)
{
	char 	*key;
	char	*value;
	int		j;
	int		seen;

	(void)env;
	seen = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			j = 0;
			key = get_value_from_key(str + i, &j);
			if (ft_strlen(key) > 0)
			{
				i += j;
				value = ft_strnewcat(read, value);
				free(key);
				key = NULL;
			}
			if (key)
				free(key);
			read = ft_strnewcat(read, &str[i++]);
		}
		i++;
	}
	if (seen == 0 && !str[i])
		return (str);
	return (read);
}
