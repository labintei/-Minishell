/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 15:38:22 by labintei          #+#    #+#             */
/*   Updated: 2021/09/09 19:38:55 by malatini         ###   ########.fr       */
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
	long i;
	long max;
	long sign;
	long result;

	i = 0;
	max = 0;
	sign = 1;
	result = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (max >= 1)
			return (0);
		if (str[i] == '-')
			sign = -sign;
		max++;
		i++;
	}
	while (ft_isdigit(str[i]))
		result = result * 10 + (str[i++] - '0');
	return ((int)result * sign);
}

/* A afficher si l argument d exit n est pas un nombre */
//int exit_error_message

/* Permet de savoir si la chaine est entierement composee de chiffres */
bool	ft_str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (false);
		i++;
	}
	return (true);
}

int		exit_build(t_env *env, t_list *cmds, bool fork)
{
	(void)fork;

	int	ret;
	fork = 1;
	if (cmds && cmds->cmds[0] && cmds->cmds[1] && cmds->cmds[2]/*cmds_length(cmds) > 2 && fork*/)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	//if (cmds && cmds_length(cmds) <= 2)
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