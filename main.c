/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:48:50 by labintei          #+#    #+#             */
/*   Updated: 2021/09/10 15:53:44 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restart_t_list_file(t_list_file		**file)
{
	while ((*file) && (*file)->previous)
		(*file) = (*file)->previous;
}

int		count_word_before_redir(char	*line, int	i);

// FACON DE RESUDRE LE PROBLEMME FAIRE COUNT CHAR AU PREALABLE

void	add_list_file(t_list_file **file, int i, char	c)
{
	t_list_file		*new;

	new = malloc(sizeof(t_list_file));
	if (!new)
		return ;
	new->next = NULL;
	new->redir = c;
	new->path = malloc(sizeof(char) * (i + 1));
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

int		find_var_and_strlen(char *line, int *j, t_env *env)
{
	char	*new;
	int		a;
	int		ret;

	a = 0;
	(*j)++;
	while (line && line[(*j) + a] && is_alphanum(line[(*j) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a  = 0;
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

int		nb_redir(char *line, int i)
{
	int		j;
	char	c;
	int		redir;
	
	redir = 0;
	j = i;
	while (line && line[j])
	{
		if ((line[j] == '\'' || line[j] == '\"') && ft_second(line[j], line, j))
		{
			c = line[j];
			j++;
			while (line[j] && line[j] != c)
				j++;
			j++;
		}
		else if (line[j] && line[j] == '|')
		{
			return (redir);
		}
		else if(line[j] && ((line[j] == '<') | (line[j] == '>')))
		{
			c = line[j];
			j++;
			redir++;
			if (line[j] && line[j] == c)
				j++;
		}
		else
			j++;
	}
	return(redir);
}

void	ajout_cmds(t_env *env, char *line, int *i)
{
	add_cmds(&(env->cmds));
	if(!(env->cmds->file))
		return ;
	env->cmds->cmds = NULL;
	env->cmds->file = NULL;
	env->cmds->cmds = malloc(sizeof(char *) * \
	(count_word(line, i) + 1 - nb_redir(line, (*i))));
}

void	is_pipe(t_env *env, int *i, int *word, char *line, int		*is_cmds)
{
	env->cmds->type = '|';
	if(*is_cmds)
		env->cmds->cmds[(*word)] = NULL;
	(*is_cmds) = 1;
	(*i)++;
	(*word) = 0;
	ajout_cmds(env, line, i);
}

int		is_redir(char *line, int *i, t_env *env)
{
	char	c;
	
	c = line[(*i)];
	(*i)++;
	if(line[(*i)] && line[(*i) - 1] && line[(*i)] == line[((*i) - 1)])
	{
		c = 'L';
		if(line[(*i)] == '>')
			c = 'R';
		(*i)++;
	}
	skip_space(line, i);
	if(line[(*i)] && (line[(*i)] == '<' || line[(*i)] == '>'))
		return(2);
	add_list_file(&(env->cmds->file), count_char(line, (*i), env), c);
	return(0);
}

int			find_var_and_strlen_cmds(char *line, t_env *env, int *count);


void		is_quotes_cmds(char *line, int *count,  t_env *env)
{
	char	c;

	c = line[(env->i)];
	(env->i)++;
	while(line[(env->i)] && line[(env->i)] != c)
	{
		if(c == '\"' && line[(env->i)] == '$')
			find_var_and_strlen_cmds(line, env, count);
		else
		{
			if(env->is_cmds)
				env->cmds->cmds[(env->word)][(*count)] = line[(env->i)];
			else
				env->cmds->file->path[(*count)] = line[(env->i)];
			(env->i)++;
			(*count)++;
		}
	}
	(env->i)++;
}

void	ft_dup_env(t_list_env	**env, char *var, char **s)
{
	int		a;
	t_list_env	*l;

	l = (*env);
	while(l)
	{
		if(l->var && ft_strcmp(l->var, var))
		{
			(*s) = malloc(sizeof(char) * (ft_strlen(l->val) + 1));
			a  = -1;
			while(l->val && l->val[(++a)])
				(*s)[a] = l->val[a];
			(*s)[a] = '\0';
			return ;
		}
		l = l->next;
	}
}

int			find_var_and_strlen_cmds(char *line, t_env *env, int *count)
{
	char	*new;
	char	*stock;
	int	a;
	int	ret;

	a = 0;
	ret = 0;
	(env->i)++;
	while(line && line[(env->i) + a] && is_alphanum(line[(env->i) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a  = 0;
	while(line && line[(env->i)] && is_alphanum(line[(env->i)]))
	{
		new[a] = line[(env->i)];
		a++;
		(env->i)++;
	}
	new[a] = '\0';
	printf("\nNEW %s\n", new);
	stock = NULL;
	ft_dup_env(&(env->env), new, &stock);
	a = 0;
	while(stock && stock[a])
	{
		if(env->is_cmds)
			env->cmds->cmds[(env->word)][(*count)] = stock[a];
		else
			env->cmds->file->path[(*count)] = stock[a];
		(*count)++;
		a++;
	}
	if(new)
		free(new);
	if(stock)
		free(stock);
	return(ret);
}

int		is_only_space(char *line)
{
	int  i;

	i = 0;
	while(line && line[i] == ' ')
		i++;
	if(line[i] == '\0')
		return(1);
	return(0);
}

void	not_quotes_cmds(char *line, t_env *env, int *count, int *out)
{
	while(line[(env->i)] && (!(is_quotes(line[(env->i)], line, (env->i)))) && \
	line[(env->i)] != '|' && line[(env->i)] != ' ' && line[(env->i)] != '<' && \
	line[(env->i)] != '>')
	{
		if(line[(env->i)] && line[(env->i)] == '$')
			find_var_and_strlen_cmds(line, env, count);
		else
		{
			if(env->is_cmds == 1)
				env->cmds->cmds[(env->word)][(*count)] = line[(env->i)];
			else
				env->cmds->file->path[(*count)] = line[(env->i)];
			(*count)++;
			env->i++;
		}
	}
	if(line[(env->i)] && line[(env->i)] == '|')
		*out = 1;
	if(line[(env->i)] && (line[(env->i)] == '<' || line[(env->i)] == '>'))
		*out = 1;

}

void		is_word_cmds(char *line, int *i, t_env *env)
{
	int		count;
	int		out;

	out  = 0;
	count = 0;
	if((env->is_cmds))
	{
		if(!env->cmds->cmds)
			env->cmds->cmds = malloc(sizeof(char *) * (count_word(line, i) - nb_redir(line, (*i)) + 1));
		env->cmds->cmds[(env->word)] = malloc(sizeof(char) * (count_char(line, (*i), env) + 1));
	}
	if(line[(*i)] && line[(*i)] != '|' && line[(*i)] != ' ')
	{
		while(line[(*i)] && line[(*i)] != ' ' && out == 0)
		{
			if(is_quotes(line[(*i)], line, (*i)))
				is_quotes_cmds(line, &count, env);
			else
				not_quotes_cmds(line, env, &count, &out);
		}
	}
	if((env->is_cmds))
		env->cmds->cmds[(env->word)][count] = '\0';
	else
		env->cmds->file->path[count] = '\0';
	if((env->is_cmds))
		(env->word)++;
	if((env->is_cmds) == 0)
		(env->is_cmds) = 1;
}

int		count_redir(char *line, int j)
{
	int		i;
	int		redir;
	char	c;
	int		stop;

	redir = 0;
	i = j;
	stop = 0;
	while(line[i] && stop == 0)
	{
		if(line[i] && stop == 0 && (line[i] == '\'' || line[i] == '\"') && ft_second(line[i], line, i))
		{
			c = line[i];
			i++;
			while(line[i] && line[i] != c)
				i++;
			if(line[i] == c)
				i++;
		}
		else
		{
			while(line[i] && stop == 0 && !((line[i] == '\'' || line[i] == '\"') && ft_second(line[i], line, i)))
			{
				if(line[i] && line[i] == '|')
					stop = 1;
				else if(line[i] && (line[i] == '<' || line[i] == '>'))
				{
					c = line[i];
					i++;
					if(line[i] == c)
						i++;
					redir++;
				}
				i++;
			}
		}
	}
	return(redir);
}

int			count_word_before_redir(char	*line, int  j)
{
	int			word;
	int			i;
	char		stop;
	char		stop_bis;
	char		c;

	stop_bis = 0;
	stop = 0;
	i = j;
	word = 0;
	while(line[i] && stop == 0)
	{
		if(line[i] && line[i] == '|')
			stop = 1;
		else if(line[i] && line[i] == ' ' && stop == 0)
		{
			while(line[i] && line[i] == ' ')
				i++;
		}
		else if(line[i] && stop == 0)
		{
			if(line[i] && (line[i] != '<' && line[i] != '>'))
				word++;
			stop_bis = 0;
			while(line[i] && stop_bis == 0)
			{
				if(line[i] && ((line[i] == '\'' || line[i] == '\"') && ft_second(line[i], line, i)))
				{
					c = line[i];
					i++;
					while(line[i] && line[i] != c)
						i++;
					if(line[i] && line[i] == c)
						i++;
				}
				else
				{
					while(line[i] && (!((line[i] == '\'' || line[i] == '\"') && ft_second(line[i], line, i))) && stop_bis == 0)
					{
						if(line[i] && line[i] == ' ')
							stop_bis = 1;
						else if(line[i] == '>' || line[i] == '<')
						{
							return(word);
						}
						i++;
					}
				}
			}
		}
	}
	return(word);
}

void	parse_line(t_env *env, char *line)
{
	if(!line || line[0] == '\0' || is_only_space(line))
	{
		env->cmds = NULL;
		return ;
	}
	env->cmds = NULL;
	env->i = 0;
	env->word = 0;
	env->is_cmds = 1;
	ajout_cmds(env, line, &(env->i));
	while(line && line[(env->i)])
	{
		skip_space(line, &(env->i));
		if(line[(env->i)] && line[(env->i)] == '|')
			is_pipe(env, &(env->i), &(env->word), line, &(env->is_cmds));
		else if(line[(env->i)] && line[(env->i)] != '|' && line[(env->i)] != ' ')
		{
			if(line[(env->i)] && (line[(env->i)] == '>' || line[(env->i)] == '<'))
			{
				is_redir(line, &(env->i), env);
				env->is_cmds = 0;
			}
			else
				is_word_cmds(line, &(env->i), env);
		}
	}
	if(env->is_cmds && env->cmds && env->cmds->cmds)
		env->cmds->cmds[(env->word)] = NULL;
}

int		start_parse(t_env	*env)
{
	char	*line;

	while (true)
	{
		line = readline("\033[1;36muser@minishell$ \033[0m$ ");
		if (line)
		{
			parse_line(env, line);
			if(env->cmds)
			{
				exec_cmds(env);
				clear_cmds(&(env->cmds));
			}
		}
		add_history(line);
		if (!line)
		{
			printf("\n");
			clear_cmds(&(env->cmds));
		}
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
}

int		main(int argc, char **argv, char **envp)
{
	t_env	env;
	int		ret;

	(void)argv;
	ret = 0;
	if (argc == 1)
	{
		ascii_art();
		env.split_path = NULL;
		stock_env(&env, envp);
		get_splitted_path(&env);
		env.cmds = NULL;
//		handle_signals();
		ret = start_parse(&env);
	}
	return (ret);
}
