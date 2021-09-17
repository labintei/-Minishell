/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:35:25 by labintei          #+#    #+#             */
/*   Updated: 2021/09/17 17:35:29 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* retours a verifier, tests */
bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

int	ft_atoi(const char *str)
{
	int i;
	int max;
	int sign;
	int result;

	i = 0;
	max = 0;
	sign = 1;
	result = 0;
	while (str && str[i] && (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' '))
		i++;
	while (str && str[i] && (str[i] == '-' || str[i] == '+') && \
	(result >= INT_MIN && result <= INT_MAX))
	{
		if (max >= 1)
			return (0);
		if (str[i] == '-')
			sign = -sign;
		max++;
		i++;
	}
	while (str && str[i] && ft_isdigit((int)str[i]))
		result = result * 10 + (str[i++] - '0');
	return (((int)result) * ((int)sign));
}

/* A afficher si l argument d exit n est pas un nombre */
//int exit_error_message

/* Permet de savoir si la chaine est entierement composee de chiffres */
bool	ft_str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (ft_isdigit((int)str[i]) == 0)
			return (false);
		i++;
	}
	return (true);
}

int		exit_build(t_env *env, t_list *cmds, bool fork)
{
	int	ret;
	fork = 1;
	if (cmds && cmds->cmds[0] && cmds->cmds[1] && cmds->cmds[2]/*cmds_length(cmds) > 2 && fork*/)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else
	{
		if (ft_str_is_digit(cmds->cmds[1]) == true)
			ret = ft_atoi(cmds->cmds[1]);
		else
		{
			if (fork)
				printf("bash: exit: %s: numeric argument required\n", cmds->cmds[1]);
			return (1);
		}
	}
	if (fork)
		printf("exit\n");
	clear_env(env);
	exit (ret);
	return (ret);
}
