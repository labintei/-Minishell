/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:48:50 by labintei          #+#    #+#             */
/*   Updated: 2021/09/23 20:33:08 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restart_t_list_file(t_list_file **file)
{
	while ((*file) && (*file)->previous)
		(*file) = (*file)->previous;
}

void	add_list_file(t_list_file **file, int i, char c)
{
	t_list_file		*new;

	new = malloc(sizeof(t_list_file));
	if (!new)
		return ;
	new->next = NULL;
	new->redir = c;
	new->is_quotes = 0;
	new->ambigous = NULL;
	if(c != 'L')
		new->path = malloc(sizeof(char) * (i + 1));
	else
		new->path = NULL;
	if (*file)
	{
		(*file)->next = new;
		new->previous = (*file);
		(*file) = (*file)->next;
	}
	else
	{
		new->previous = NULL;
		(*file) = new;
	}
}

int	find_var_and_strlen(char *line, int *j, t_env *env)
{
	char	*new;
	int		a;
	int		ret;

	a = 0;
	(*j)++;
	while (line && line[(*j) + a] && is_alphanum(line[(*j) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a = 0;
	while (line && line[(*j)] && is_alphanum(line[(*j)]))
	{
		new[a] = line[(*j)];
		a++;
		(*j)++;
	}
	new[a] = '\0';
	ret = ft_strlen_env(&(env->env), new);
	if (new)
		free(new);
	return (ret);
}

void	skip_quotes(char *line, int *j)
{
	char	c;

	if (line && line[(*j)] && is_quotes(line[(*j)], line, (*j)))
	{
		c = line[(*j)];
		(*j)++;
		while (line[(*j)] && line[(*j)] != c)
			(*j)++;
		(*j)++;
	}
}

int			next_is_word(char *line, int i);


int	nb_redir(char *line, int h)
{
	int		i[3];

	i[1] = 0;
	i[0] = h;
	while (line && line[i[0]])
	{
		if (line[i[0]] && is_quotes(line[i[0]], line, i[0]))
			skip_quotes(line, &(i[0]));
		else if (line[i[0]] && line[i[0]] == '|')
		{
			return (i[1]);
		}
		else if (line[i[0]] && ((line[i[0]] == '<') | (line[i[0]] == '>')))
		{
			i[2] = line[i[0]];
			(i[0])++;
			if (line[i[0]] && line[i[0]] == (char)i[2])
				(i[0])++;
			if(next_is_word(line, (i[0])))
				(i[1])++;
		}
		else
			(i[0])++;
	}
	return (i[1]);
}

void	ajout_cmds(t_env *env, char *line, int *i)
{
	add_cmds(&(env->cmds));
	if (!(env->cmds->file))
		return ;
	env->cmds->cmds = NULL;
	env->cmds->file = NULL;
	if(((count_word(line, i, env) + 1) - nb_redir(line, (*i))) > 0)
		env->cmds->cmds = malloc(sizeof(char *) * \
		(count_word(line, i, env) + 1 - nb_redir(line, (*i))));
//	else
//		env->cmds->cmds = malloc(sizeof(char *) * 2);
}

void	is_pipe(t_env *env, char *line)
{
	env->cmds->type = '|';
	if(env->empty == 1 && env->error == 0)
	{
		env->error = 1;
		env->none_ex = (line[(env->i)]);
//		ft_putstr_fd("\nError unexpected token :", 2); %c\n", env->none_ex);
	}
	if (env->is_cmds && env->cmds && env->cmds->cmds)
		env->cmds->cmds[env->word] = NULL;
	env->is_cmds = 1;
	(env->i)++;
	env->word = 0;
	ajout_cmds(env, line, &(env->i));
	env->empty = 1;
}

int	is_redir(char *line, int *i, t_env *env)
{
	char	c;

	c = line[(*i)];
	(*i)++;
	if (line[(*i)] && line[(*i) - 1] && line[(*i)] == line[((*i) - 1)])
	{
		c = 'L';
		if (line[(*i)] == '>')
			c = 'R';
		(*i)++;
	}
	if((env->last_type == 'r') && env->error == 0)
	{
		env->error = 1;
		env->none_ex = c;
//		printf("\nError unexpected token : %c\n", env->none_ex);
	}
	env->last_type = 'r';
	skip_space(line, i);
	if (line[(*i)] && (line[(*i)] == '<' || line[(*i)] == '>'))
		return (2);
	add_list_file(&(env->cmds->file), count_char(line, (*i), env), c);
	return (0);
}

void	is_quotes_cmds(char *line, int *count, t_env *env)
{
	char	c;

	c = line[(env->i)];
	(env->i)++;
	while (line[(env->i)] && line[(env->i)] != c)
	{
		if (c == '\"' && line[(env->i)] == '$')
			find_var_and_strlen_cmds(line, env, count);
		else
		{
			if (env->is_cmds)
				env->cmds->cmds[(env->word)][(*count)] = line[(env->i)];
			else
				env->cmds->file->path[(*count)] = line[(env->i)];
			(env->i)++;
			(*count)++;
		}
	}
	(env->i)++;
}

void	ft_dup_env(t_list_env **env, char *var, char **s)
{
	int			a;
	t_list_env	*l;

	l = (*env);
	while (l)
	{
		if (l->var && ft_strcmp(l->var, var))
		{
			(*s) = malloc(sizeof(char) * (ft_strlen(l->val) + 1));
			a = -1;
			while (l->val && l->val[(++a)])
				(*s)[a] = l->val[a];
			(*s)[a] = '\0';
			return ;
		}
		l = l->next;
	}
}

int	find_var_and_strlen_cmds(char *line, t_env *env, int *count)
{
	char	*new;
	char	*stock;
	int		a;
	int		ret;

	a = 0;
	ret = 0;
	(env->i)++;
	while (line && line[(env->i) + a] && is_alphanum(line[(env->i) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
//	if(!new)
//		exit_fatal(1, env);
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
//		exit_fatal(1, env);
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
	if (new)
		free(new);
	if (stock)
		free(stock);
	return (ret);
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

int		save_ambigous(char **stock, char *line, int i)
{
	int		j;

	j = (i);
	while(line[j] && line[j] != '|' && line[j] != '<' && line[j] != '>' && line[j] != ' ')
		j++;
	(*stock) = malloc(sizeof(char) * ((j - (i)) + 1));
	if(!(*stock))
		return(0);
	j = 0;
	while(line[(i)] && line[(i)] != '|' && line[(i)] != '<' && line[(i)] != '>' && line[(i)] != ' ')
	{
		(*stock)[j] = line[(i)];
		j++;
		++(i);
	}
	(*stock)[j] = '\0';
	return(1);
}

int		is_only_var(char *line, int i)
{
	int		j;

	j = i;
	j++;
	while(line[j] && line[j] != '|' && line[j] != '<' && line[j] != '>' && line[j] != ' ')
	{
		if(line[j] == '\'' || line[j] == '\"')
			return(0);
		j++;
	}
	return(1);
}

void	parse_word_heredoc(char *line, int *i, t_env *env, int *count)
{
	int		j;
	int		quotes;
	char	c;

	quotes = 0;
	j = 0;
	while(line[(*i) + j] && line[(*i) + j] != '|' && line[(*i)+ j] != '<' && line[(*i) + j] != '>' && line[(*i) + j] != ' ')
	{
		if(line[(*i) + j] && ((line[(*i) + j] == '\'' || line[(*i) + j] == '\"') && ft_second(line[(*i) + j], line, ((*i) + j))))
		{
			c = line[(*i) + j];
			j++;
			quotes++;
			while(line[(*i) + j] && line[(*i) + j] != c)
				j++;
			j++;
			quotes++;
		}
		else
			j++;
	}
	env->cmds->file->path = malloc(sizeof(char) * ((j - quotes) + 1));
	if(!(env->cmds->file->path))
		exit_fatal(1, env);
	(*count) = 0;
	while(line[(*i)] && line[(*i)] != '|' && line[(*i)] != '<' && line[(*i)] != '>' && line[(*i)] != ' ')
	{
		if(line[(*i)] && ((line[(*i)] == '\'' || line[(*i)] == '\"') && ft_second(line[(*i)], line, ((*i)))))
		{
			c = line[(*i)];
			env->cmds->file->is_quotes = 1;
			(*i)++;
			while(line[(*i)] && line[(*i)] != c)
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
//	env->cmds->file->path[j] = '\0';
}

// Il y avait des erreur
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
	//	if(!env->cmds->cmds)
	//		exit_fatal(1, env);
		if(env->cmds->cmds)
			env->cmds->cmds[(env->word)] = malloc(sizeof(char) * \
			(count_char(line, (*i), env) + 1));
	//	if(!env->cmds->cmds[(env->word)])
	//		exit_fatal(1, env);
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
		{
			parse_word_heredoc(line, i, env, &count);
		}
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
	{
		env->cmds->file->path[count] = '\0';
	//	if(env->cmds->file->path)
	//		printf("\n%s\n", env->cmds->file->path);
	}
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

int	count_redir(char *line, int j)
{
	int		i;
	int		redir;
	char	c;
	int		stop;

	redir = 0;
	i = j;
	stop = 0;
	while (line[i] && stop == 0)
	{
		if (line[i] && stop == 0 && (line[i] == '\'' || line[i] == '\"') && ft_second(line[i], line, i))
		{
			c = line[i];
			i++;
			while (line[i] && line[i] != c)
				i++;
			if (line[i] == c)
				i++;
		}
		else
		{
			while (line[i] && stop == 0 && !((line[i] == '\'' || line[i] == '\"') && ft_second(line[i], line, i)))
			{
				if (line[i] && line[i] == '|')
					stop = 1;
				else if (line[i] && (line[i] == '<' || line[i] == '>'))
				{
					c = line[i];
					i++;
					if (line[i] == c)
						i++;
					if(next_is_word(line , i))
						redir++;
				}
				i++;
			}
		}
	}
	return (redir);
}

void	parse_line(t_env *env, char *l)
{
	env->cmds = NULL;
	if (!l || l[0] == '\0' || is_only_space(l))
	{
		env->cmds = NULL;
		return ;
	}
	env->cmds = NULL;
	env->error = 0;
	env->empty = 1;
	env->none_ex = 0;
	env->i = 0;
	env->word = 0;
	env->is_cmds = 1;
	env->last_type = 0;
	ajout_cmds(env, l, &(env->i));
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
	if (/*env->is_cmds &&*/ env->cmds /*&& (env->word) > 0*/ && env->cmds->cmds /*&& env->cmds->cmds[(env->word) - 1]*/)
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
		if (line)
		{
			parse_line(env, line);
		}
		if (env->cmds && error_unexpected(env->error, env))
		{
			exec_cmds(env);
			clear_cmds(&(env->cmds));
		}
		else if (env->cmds)
			clear_cmds(&(env->cmds));
		add_history(line);
		if (!line)
		{
			printf("\n");
			clear_cmds(&(env->cmds));
		}
		if(env->split_path)
			clear_tab(&(env->split_path));
		get_splitted_path(env);
		if (line)
			free(line);
	}
	return (1);
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

int		STATUS;

int	main(int argc, char **argv, char **envp)
{
	t_env	env;
	int		ret;

	(void)argv;
	ret = 0;
	if (argc == 1)
	{
//		STATUS = 0;
//		define_signals();
		handle_signals();
		ascii_art();
		env.split_path = NULL;
		stock_env(&env, envp);
		get_splitted_path(&env);
		handle_signals();
		env.cmds = NULL;
		ret = start_parse(&env);
	}
	return (ret);
}
