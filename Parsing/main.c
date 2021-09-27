/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:48:50 by labintei          #+#    #+#             */
/*   Updated: 2021/09/27 18:58:36 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_var_and_strlen_2(char *stock, t_env *env, int *count)
{
	int		a;

	a = 0;
	while (stock && stock[a])
	{
		if (env->is_cmds)
			env->cmds->cmds[(env->word)][(*count)] = stock[a];
		else
			env->cmds->file->path[(*count)] = stock[a];
		(*count)++;
		a++;
	}
}

void	find_var_and_strlen_cmds(char *line, t_env *env, int *count)
{
	char	*new;
	char	*stock;
	int		a;

	a = 0;
	(env->i)++;
	while (line && line[(env->i) + a] && is_alphanum(line[(env->i) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a = 0;
	while (line && line[(env->i)] && is_alphanum(line[(env->i)]))
	{
		new[a] = line[(env->i)];
		a++;
		(env->i)++;
	}
	new[a] = '\0';
	stock = NULL;
	ft_dup_env(&(env->env), new, &stock);
	find_var_and_strlen_2(stock, env, count);
	if (new)
		free(new);
	if (stock)
		free(stock);
}

int	is_only_space(char *line)
{
	int	i;

	i = 0;
	while (line && line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}

void	not_quotes_cmds(char *line, t_env *env, int *count, int *out)
{
	while (line[(env->i)] && (!(is_quotes(line[(env->i)], line, (env->i)))) && \
	line[(env->i)] != '|' && line[(env->i)] != ' ' && line[(env->i)] != '<' && \
	line[(env->i)] != '>')
	{
		if (line[(env->i)] && line[(env->i)] == '$')
			find_var_and_strlen_cmds(line, env, count);
		else
		{
			if (env->is_cmds == 1)
				env->cmds->cmds[(env->word)][(*count)] = line[(env->i)];
			else
				env->cmds->file->path[(*count)] = line[(env->i)];
			(*count)++;
			env->i++;
		}
	}
	if (line[(env->i)] && line[(env->i)] == '|')
		*out = 1;
	if (line[(env->i)] && (line[(env->i)] == '<' || line[(env->i)] == '>'))
		*out = 1;
}

int	save_ambigous(char **stock, char *line, int i)
{
	int		j;

	j = (i);
	while (line[j] && line[j] != '|' && line[j] != '<' && line[j] != '>' && line[j] != ' ')
		j++;
	(*stock) = malloc(sizeof(char) * ((j - (i)) + 1));
	if (!(*stock))
		return (0);
	j = 0;
	while(line[(i)] && line[(i)] != '|' && line[(i)] != '<' && line[(i)] != '>' && line[(i)] != ' ')
	{
		(*stock)[j] = line[(i)];
		j++;
		++(i);
	}
	(*stock)[j] = '\0';
	return (1);
}

int	is_only_var(char *line, int i)
{
	int	j;

	j = i;
	j++;
	while (line[j] && line[j] != '|' && line[j] != '<' && line[j] != '>' && line[j] != ' ')
	{
		if (line[j] == '\'' || line[j] == '\"')
			return (0);
		j++;
	}
	return (1);
}

void	parse_word_heredoc_0(char *line, int *i, t_env *env)
{
	int		quotes;
	int		j;
	char	c;

	quotes = 0;
	j = 0;
	while (line[(*i) + j] && line[(*i) + j] != '|' && line[(*i)+ j] != '<'\
	&& line[(*i) + j] != '>' && line[(*i) + j] != ' ')
	{
		if (line[(*i) + j] && ((line[(*i) + j] == '\''|| line[(*i) + j] == '\"') && ft_second(line[(*i) + j], line, ((*i) + j))))
		{
			c = line[(*i) + j];
			j++;
			quotes++;
			while (line[(*i) + j] && line[(*i) + j] != c)
				j++;
			j++;
			quotes++;
		}
		else
			j++;
	}
	env->cmds->file->path = malloc(sizeof(char) * ((j - quotes) + 1));
	if (!(env->cmds->file->path))
		exit_fatal(1, env);
}

void	parse_word_heredoc(char *line, int *i, t_env *env, int *count)
{
	char	c;

	parse_word_heredoc_0(line, i, env);
	(*count) = 0;
	while (line[(*i)] && line[(*i)] != '|' && line[(*i)] != '<' && line[(*i)] != '>' && line[(*i)] != ' ')
	{
		if (line[(*i)] && ((line[(*i)] == '\'' || line[(*i)] == '\"') && ft_second(line[(*i)], line, ((*i)))))
		{
			c = line[(*i)];
			env->cmds->file->is_quotes = 1;
			(*i)++;
			while (line[(*i)] && line[(*i)] != c)
			{
				env->cmds->file->path[(*count)] = line[(*i)];
				(*count)++;
				(*i)++;
			}
			(*i)++;
		}
		else
		{
			env->cmds->file->path[(*count)] = line[(*i)];
			(*count)++;
			(*i)++;
		}
	}
}

void	is_word_cmds(char *line, int *i, t_env *env)
{
	int		count;
	int		out;

	out = 0;
	count = 0;
	if ((env->is_cmds))
	{
		if (!env->cmds->cmds && (count_word(line, i, env) - nb_redir(line, (*i))) > 0)
			env->cmds->cmds = malloc(sizeof(char *) * \
			(count_word(line, i, env) - nb_redir(line, (*i)) + 1));
		if(env->cmds->cmds)
			env->cmds->cmds[(env->word)] = malloc(sizeof(char) * \
			(count_char(line, (*i), env) + 1));
	}
	if(!env->is_cmds && env->cmds->error == 0 && env->last_type == 'r' && (env->cmds->file && env->cmds->file->redir != 'L') && line[(*i)] && line[(*i)] == '$' && \
	is_only_var(line, (*i))  && count_char(line, (*i), env) == 0)
	{
		if(!(save_ambigous(&(env->cmds->file->ambigous), line, (*i))))
			exit_fatal(1, env);
	}
	if (line[(*i)] && line[(*i)] != '|' && line[(*i)] != ' ')
	{
		if(!env->is_cmds && env->last_type == 'r' && env->cmds->file && env->cmds->file->redir == 'L')
			parse_word_heredoc(line, i, env, &count);
		else
		{
			while (line[(*i)] && line[(*i)] != ' ' && out == 0)
			{
				if (is_quotes(line[(*i)], line, (*i)))
					is_quotes_cmds(line, &count, env);
				else
					not_quotes_cmds(line, env, &count, &out);
			}
		}
	}
	if ((env->is_cmds))
		env->cmds->cmds[(env->word)][count] = '\0';
	else
		env->cmds->file->path[count] = '\0';
	if ((env->is_cmds))
		(env->word)++;
	if ((env->is_cmds) == 0)
		(env->is_cmds) = 1;
}

int			next_is_word(char *line, int i)
{
	while(line && line[i])
	{
		if(line && line[i] == ' ')
			i++;
		else if(line && (line[i] == '<' || line[i] == '>' || line[i] == '|'))
			return(0);
		else
			return(1);
	}
	return(0);
}

void	init_parse_line(t_env *env, char *line)
{
	env->cmds = NULL;
	env->error = 0;
	env->empty = 1;
	env->none_ex = 0;
	env->i = 0;
	env->word = 0;
	env->is_cmds = 1;
	env->last_type = 0;
	ajout_cmds(env, line, &(env->i));
}

void	parse_line(t_env *env, char *l)
{
	env->cmds = NULL;
	if (!l || l[0] == '\0' || is_only_space(l))
		return ;
	init_parse_line(env, l);
	while (l && l[(env->i)])
	{
		skip_space(l, &(env->i));
		if (l[(env->i)] && l[(env->i)] == '|')
			is_pipe(env, l);
		else if (l[(env->i)] && l[(env->i)] != '|' && l[(env->i)] != ' ')
		{
			if (l[(env->i)] && (l[(env->i)] == '>' || l[(env->i)] == '<'))
			{
				is_redir(l, &(env->i), env);
				env->is_cmds = 0;
			}
			else
			{
				env->empty = 0;
				is_word_cmds(l, &(env->i), env);
				env->last_type = 'w';
			}
		}
	}
	if (env->cmds && env->cmds->cmds)
		env->cmds->cmds[(env->word)] = NULL;
	if(env->last_type == 'r'  && env->error == 0)
	{
		env->error = 1;
		env->none_ex = 'n';
	}
}

int	start_parse(t_env *env)
{
	char	*line;

	while (true)
	{
		line = readline("\033[1;36muser@minishell$ \033[0m$ ");
		if(!line)
		{
			ft_putstr_fd("\b\b", 0);
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (line)
			parse_line(env, line);
		if (env->cmds && error_unexpected(env->error, env))
		{
			exec_cmds(env);
			clear_cmds(&(env->cmds));
		}
		else if (env->cmds)
			clear_cmds(&(env->cmds));
		add_history(line);
		if (!line)
			printf("\n");
		if(env->split_path)
			clear_tab(&(env->split_path));
		get_splitted_path(env);
		if (line)
			free(line);
	}
	return (RET);
}

void	init_env(t_env *env)
{
	env->cmds = NULL;
	env->env = NULL;
	env->split_path = NULL;
	env->empty = 1;
	env->error = 0;
	env->none_ex = 0;
}

int		RET;

int	main(int argc, char **argv, char **envp)
{
	t_env	env;

	(void)argv;
	RET = 0;
	if (argc == 1)
	{
		handle_signals();
		ascii_art();
		env.split_path = NULL;
		stock_env(&env, envp);
		get_splitted_path(&env);
		env.cmds = NULL;
		start_parse(&env);
	}
	clear_env(&env);
	return (RET);
}
