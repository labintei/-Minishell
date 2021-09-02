/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:48:50 by labintei          #+#    #+#             */
/*   Updated: 2021/09/02 14:14:25 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		find_var_and_strlen(char *line, int *j, t_env *env)
{
	char	*new;
	int		a;
	int		ret;

	a = 0;
	(*j)++;
	while(line && line[(*j) + a] && is_alphanum(line[(*j) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a  = 0;
	while(line && line[(*j)] && is_alphanum(line[(*j)]))
	{
		new[a] = line[(*j)];
		a++;
		(*j)++;
	}
	new[a] = '\0';
	ret = ft_strlen_env(&(env->env), new);
	if(new)
		free(new);
	return(ret);
}

void		ajout_cmds(t_env *env, char *line, int *i)
{
	add_cmds(&(env->cmds));
	env->cmds->cmds = malloc(sizeof(char *) * (count_word(line, i) + 1));
	env->cmds->cmds_type = malloc(sizeof(char) * (count_word(line, i) + 1));
}

void		is_pipe(t_env *env, int *i, int *word, char *line)
{
	env->cmds->type = '|';
	env->cmds->cmds[(*word)] = NULL;
	(*i)++;
	(*word) = 0;
	ajout_cmds(env, line, i);
}

// GERER CE TYPE DE CAS < (NULL) > << >> 

int		is_redir(char *line, int *i, t_env *env, int *word)
{
	env->cmds->cmds_type[(*word)] = line[(*i)];
	if(!(env->cmds->cmds[(*word)]))
		env->cmds->cmds[(*word)] = NULL;
	(*i)++;
	if(line[(*i)] && line[(*i) - 1] && line[(*i)] == line[((*i) - 1)])
	{
		env->cmds->cmds_type[(*word)] = 'L';
		if(line[(*i)] == '>')
			env->cmds->cmds_type[(*word)] = '<';
		(*i)++;
	}
	skip_space(line, i);
	if(line[(*i)] && (line[(*i)] == '<' || line[(*i)] == '>'))
		return(2);
	return(0);
}

int			find_var_and_strlen_cmds(char *line, int *j, t_env *env, int *word, int *count);


void		is_quotes_cmds(char *line, int *j, int *count,  t_env *env, int *word)
{
	char	c;

	c = line[(*j)];
	(*j)++;
	while(line[(*j)] && line[(*j)] != c)
	{
		if(c == '\"' && line[(*j)] == '$')
			find_var_and_strlen_cmds(line, j, env, word, count);
		else
		{
			env->cmds->cmds[(*word)][(*count)] = line[(*j)];
			(*j)++;
			(*count)++;
		}
	}
	(*j)++;
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

int			find_var_and_strlen_cmds(char *line, int *j, t_env *env, int *word, int *count)
{
	char	*new;
	char	*stock;
	int	a;
	int	ret;

	a = 0;
	ret = 0;
	(*j)++;
	while(line && line[(*j) + a] && is_alphanum(line[(*j) + a]))
		a++;
	new = malloc(sizeof(char) * (a + 1));
	a  = 0;
	while(line && line[(*j)] && is_alphanum(line[(*j)]))
	{
		new[a] = line[(*j)];
		a++;
		(*j)++;
	}
	new[a] = '\0';
	printf("\nNEW %s\n", new);
	stock = NULL;
	ft_dup_env(&(env->env), new, &stock);
//	if(stock)
//		printf("\nSTOCK %s\n", stock);
	a = 0;
	while(stock && stock[a])
	{
		printf("\n %c \n", stock[a]);
		env->cmds->cmds[(*word)][(*count)] = stock[a];
		(*count)++;
		a++;
	}
	if(new)
		free(new);
	if(stock)
		free(stock);
	return(ret);
}



void		not_quotes_cmds(char *line, int *j, t_env *env, int *count, int *out, int *word)
{
	while(line[(*j)] && (!(is_quotes(line[(*j)], line, (*j)))) && \
	line[(*j)] != '|' && line[(*j)] != ' ' && line[(*j)] != '<' && \
	line[(*j)] != '>')
	{
		// REGARDER find var strlen cmds
		if(line[(*j)] && line[(*j)] == '$')
			find_var_and_strlen_cmds(line, j, env, word, count);
		else
		{
//			printf("\n\n %c \n\n",line[(*j)]);
			env->cmds->cmds[(*word)][(*count)] = line[(*j)];
			(*count)++;
			(*j)++;
		}
	}
	if(line[(*j)] && line[(*j)] == '|')
		*out = 1;
	if(line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
		*out = 1;

}

void		is_word_cmds(char *line, int *i, t_env *env, int *word)
{
	int		count;
	int		out;

	out  = 0;
	count = 0;
	env->cmds->cmds[(*word)] = malloc(sizeof(char) * (count_char(line, (*i), env) + 1));
	if(line[(*i)] && line[(*i)] != '|' && line[(*i)] != ' ')
	{
		while(line[(*i)] && line[(*i)] != ' ' && out == 0)
		{
			if(is_quotes(line[(*i)], line, (*i)))
				is_quotes_cmds(line, i, &count, env, word);
			else
				not_quotes_cmds(line, i, env, &count, &out, word);
		}
	}
	env->cmds->cmds[(*word)][count] = '\0';
	(*word)++;
}

void		parse_line(t_env *env, char *line)
{
	int	i;
	int	word;

	i = 0;
	word = 0;
//	printf("\n%d\n", count_char(line, i, env));
	ajout_cmds(env, line, &i);
	while(line && line[i])
	{
		skip_space(line, &i);
		if(line[i] && line[i] == '|')
			is_pipe(env, &i, &word, line);
		else if(line[i] && line[i] != '|' && line[i] != ' ')
		{
	//		env->cmds->cmds[(++word)] = malloc(sizeof(char) * (count_char(line, i, env) + 1));
			if(line[i] && (line[i] == '>' || line[i] == '<'))
			{
				is_redir(line, &i, env, &word);
			}
			else
				is_word_cmds(line, &i, env, &word);
		}
	}
//	if(env->cmds->cmds[(word)])
	//	env->cmds->cmds[((word) + 1)] = NULL;
	//else
	env->cmds->cmds[(word)] = NULL;
	view_cmds(&(env->cmds));
}

int			start_parse(t_env	*env)
{
	char	*line;

	while(1)
	{
		line = readline("\n>>");
		if(line)
		{
			parse_line(env, line);
			exec_cmds(env);
			clear_cmds(&(env->cmds));
		}
		add_history(line);
		if (!line)
		{
			free_mem(env);
		}
		if (line)
			free(line);
	}
	return (1);
}

void		init_env(t_env *env)
{
	env->cmds = NULL;
	env->env = NULL;
	env->split_path = NULL;
}

/* Main mahaut tests */
int			main(int argc, char **argv, char **envp)
{
	t_env	env;
	int		ret;

	(void)argv;
	ret = 0;
	if (argc == 1)
	{
		env.split_path = NULL;
		stock_env(&env, envp);
		get_splitted_path(&env);
		env.cmds = NULL;
		ret = start_parse(&env);
	}
	return (ret);
}

/* Main lauranne 
int			main(int argc, char **argv, char **envp)
{
	t_env	env;
	int		ret;
//	char	**tab;

	(void)argc;
	(void)argv;
	view_tab(envp);
//	env = malloc(sizeof(t_env));
//	if(!env)
//		return(1);
//	env->env = malloc(sizeof(t_list_env));
//	env->env = NULL;
//	init_env(env);
	env.split_path = NULL;
	stock_env(&env, envp);
//	printf("\n\nAAAAAAAAAAAAAAAAA\n\n");
	get_splitted_path(&env);
	env.cmds = NULL;
//	ft_convert_env(&(env->env), &tab);
//	view_list_env(&(env->env));
//	view_tab(tab);
//	stock_splitted_path(&list);

//	view_list_env(&(env->env));
	ret = start_parse(&env);
	return(ret);
}
*/