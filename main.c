/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:48:50 by labintei          #+#    #+#             */
/*   Updated: 2021/09/07 16:45:11 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		restart_t_list_file(t_list_file		**file)
{
	while((*file) && (*file)->previous)
		(*file) = (*file)->previous;
}


//void		list_cmds_restart(t_list	**cmds)
//int				count_redir(char *line, int j);
/*
void		view_t_list_file(t_list_file	**read)
{
	t_list_file		*temp;
	
	restart_t_list_file(read);
	temp = *read;
	printf("\nLIST FILE PATH :\n");
	while(temp)
	{
		if(temp && temp->path)
		{
			view_tab(temp->path);
			printf("\n");
		}
		temp = temp->next;
	}
}*/

int			count_word_before_redir(char	*line, int  i);

/*
void	view_all_redir(t_env *env)
{
	t_list		*r;
	int			i;

	i = 0;
	list_cmds_restart(&(env->cmds));
	r = (env->cmds);
	while(r)
	{
		printf("\nCmds %d\n", i);
		printf("\nLes differents types %s\n", r->cmds_type);
		r = r->next;
		i++;
	}
}*/

void	add_list_file(t_list_file **file, char *line, int i, char	c)
{
	t_list_file		*new;

	new = malloc(sizeof(t_list_file));
	if(!new)
		return ;
	new->next = NULL;
	new->redir = c;
	new->path = malloc(sizeof(char *) * (count_word_before_redir(line, i) + 1));
	new->fd = malloc(sizeof(int) * (count_word_before_redir(line, i)) + 1);
	if(*file)
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

int		find_var_and_strlen(char *line, int *j, t_env *env/*, int is_cmds*/)
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
//	env->cmds->file = malloc(sizeof(t_list_file));
	if(!(env->cmds->file))
		return ;
	env->cmds->file = NULL;
//	env->cmds->file->path = malloc(sizeof(char *) * (count_word_before_redir(line, (*i)) + 1));
//	env->cmds->file->fd = malloc(sizeof(int) * (count_word_before_redir(line, (*i)) + 1));
	env->cmds->cmds = malloc(sizeof(char *) * (count_word_before_redir(line, (*i)) + 1));
//	env->cmds->file = NULL;
//	env->cmds->cmds_type = malloc(sizeof(char) * (count_word(line, i) + 1));
}

void		is_pipe(t_env *env, int *i, int *word, char *line, int		*is_cmds)
{
//	(*is_cmds) = 1;
	env->cmds->type = '|';
//	env->cmds->cmds_type[(*word)] = '\0';
	if(*is_cmds)
		env->cmds->cmds[(*word)] = NULL;
	else
		env->cmds->file->path[(*word)] = NULL;
	(*is_cmds) = 1;
	(*i)++;
	(*word) = 0;
	ajout_cmds(env, line, i);
//	env->cmds->cmds = malloc(sizeof(char *) * (count_word_before_redir(line, (*i)) + 1));
//	count_redir(line, (*i));
}

// GERER CE TYPE DE CAS < (NULL) > << >> 

int		is_redir(char *line, int *i, t_env *env, int *word, int is_cmds)
{
	char	c;
	
	if(is_cmds)
		env->cmds->cmds[(*word)]=NULL;
	else if(env->cmds->file)
		env->cmds->file->path[(*word)] = NULL;
	c = line[(*i)];
	(*i)++;
	if(line[(*i)] && line[(*i) - 1] && line[(*i)] == line[((*i) - 1)])
	{
		c = 'L';
		if(line[(*i)] == '>')
			c = '<';
		(*i)++;
	}
	skip_space(line, i);
	if(line[(*i)] && (line[(*i)] == '<' || line[(*i)] == '>'))
		return(2);
	add_list_file(&(env->cmds->file), line, (*i), c);
	(*word) = 0;
	//count_word_before_redir(line, (*i));
	return(0);
}

int			find_var_and_strlen_cmds(char *line, int *j, t_env *env, int *word, int *count, int is_cmds);


void		is_quotes_cmds(char *line, int *j, int *count,  t_env *env, int *word, int is_cmds)
{
	char	c;

	c = line[(*j)];
	(*j)++;
	while(line[(*j)] && line[(*j)] != c)
	{
		if(c == '\"' && line[(*j)] == '$')
			find_var_and_strlen_cmds(line, j, env, word, count, is_cmds);
		else
		{
			if(is_cmds)
				env->cmds->cmds[(*word)][(*count)] = line[(*j)];
			else
				env->cmds->file->path[(*word)][(*count)] = line[(*j)];
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

int			find_var_and_strlen_cmds(char *line, int *j, t_env *env, int *word, int *count, int is_cmds)
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
//		printf("\n %c \n", stock[a]);
		if(is_cmds)
			env->cmds->cmds[(*word)][(*count)] = stock[a];
		else
			env->cmds->file->path[(*word)][(*count)] = stock[a];
		(*count)++;
		a++;
	}
	if(new)
		free(new);
	if(stock)
		free(stock);
	return(ret);
}



void		not_quotes_cmds(char *line, int *j, t_env *env, int *count, int *out, int *word, int is_cmds)
{
	while(line[(*j)] && (!(is_quotes(line[(*j)], line, (*j)))) && \
	line[(*j)] != '|' && line[(*j)] != ' ' && line[(*j)] != '<' && \
	line[(*j)] != '>')
	{
		// REGARDER find var strlen cmds
		if(line[(*j)] && line[(*j)] == '$')
			find_var_and_strlen_cmds(line, j, env, word, count, is_cmds);
		else
		{
//			printf("\n\n %c \n\n",line[(*j)]);
			if(is_cmds == 1)
				env->cmds->cmds[(*word)][(*count)] = line[(*j)];
			else
				env->cmds->file->path[(*word)][(*count)] = line[(*j)];
			(*count)++;
			(*j)++;
		}
	}
	if(line[(*j)] && line[(*j)] == '|')
		*out = 1;
	if(line[(*j)] && (line[(*j)] == '<' || line[(*j)] == '>'))
		*out = 1;

}

void		is_word_cmds(char *line, int *i, t_env *env, int *word, int is_cmds)
{
	int		count;
	int		out;

//	env->cmds->cmds_type[(*word)] = '0';
	out  = 0;
	count = 0;
	if(is_cmds)
		env->cmds->cmds[(*word)] = malloc(sizeof(char) * (count_char(line, (*i), env) + 1));
	else
		env->cmds->file->path[(*word)] = malloc(sizeof(char) * (count_char(line, (*i), env) + 1));
	if(line[(*i)] && line[(*i)] != '|' && line[(*i)] != ' ')
	{
		while(line[(*i)] && line[(*i)] != ' ' && out == 0)
		{
			if(is_quotes(line[(*i)], line, (*i)))
				is_quotes_cmds(line, i, &count, env, word, is_cmds);
			else
				not_quotes_cmds(line, i, env, &count, &out, word, is_cmds);
		}
	}
	if(is_cmds)
		env->cmds->cmds[(*word)][count] = '\0';
	else
		env->cmds->file->path[(*word)][count] = '\0';
	(*word)++;
}

int				count_redir(char *line, int j)
{
	int			i;
	int			redir;
	char		c;
	int			stop;
//	char		is_quote;

//	is_quote = 0;
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
//	printf("\nNOMBRE DE REDIR : %d\n", redir);
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
//							printf("\nNB_WORD %d\n", word);
							return(word);
						}
						i++;
					}
				}
			}
		}
	}
//	printf("\nNB_WORD %d\n", word);
//	view_t_list_file(&(env->cmds->file));
	return(word);
}

// Modifier tout ca tel que
//
// TAB CMDS et TAB name DOCS
//
// probleme la redir se met uniquement sur le dernier mot
//
// sera plus somplle pour executer les commandes
//
// Faire des test sur les redirection dans Bash faire un fichier txt
//
//

void		parse_line(t_env *env, char *line)
{
	int	i;
	int	word;
	int	is_cmds;

	if(env->cmds)
		view_cmds(&(env->cmds));
	env->cmds = NULL;
	i = 0;
	word = 0;
	is_cmds = 1;
//	printf("\n%d\n", count_char(line, i, env));
	ajout_cmds(env, line, &i);
//	count_redir(line, 0);
//	count_word_before_redir(line, 0);
	while(line && line[i])
	{
		skip_space(line, &i);
		if(line[i] && line[i] == '|')
			is_pipe(env, &i, &word, line, &is_cmds);
		else if(line[i] && line[i] != '|' && line[i] != ' ')
		{
	//		env->cmds->cmds[(++word)] = malloc(sizeof(char) * (count_char(line, i, env) + 1));
			if(line[i] && (line[i] == '>' || line[i] == '<'))
			{
				is_redir(line, &i, env, &word, is_cmds);
				is_cmds = 0;
			}
			else
				is_word_cmds(line, &i, env, &word, is_cmds);
		}
	}
//	env->cmds->cmds_type[(word)] = '\0';
	if(is_cmds)
		env->cmds->cmds[(word)] = NULL;
	else
		env->cmds->file->path[(word)] = NULL;
//	view_t_list_file(&(env->cmds->file));
	view_cmds(&(env->cmds));
}



int			start_parse(t_env	*env)
{
	char	*line;

	while (true)
	{
		line = readline("Minishell$ ");
		if (line)
		{
			parse_line(env, line);
//			view_all_redir(env);
//			ft_redirection(env);
			exec_cmds(env);
			clear_cmds(&(env->cmds));
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
//		handle_signals();
		ret = start_parse(&env);
	}
	return (ret);
}
