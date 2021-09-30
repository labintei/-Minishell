/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 18:50:07 by labintei          #+#    #+#             */
/*   Updated: 2021/09/30 21:38:46 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		MINISHELL_H
#define		MINISHELL_H

#include	<limits.h>
# include <string.h>
# include <errno.h>
# include <curses.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdarg.h>

extern		int		g_ret;

#define CD_ERROR 4

typedef	struct		s_list_env
{
	char				*var;
	char				*val;
	struct	s_list_env	*next;
	struct	s_list_env	*previous;
}					t_list_env;

/* Attention entre les fd du file et les fd de la cmd */
typedef	struct		s_list_file
{
	char					*path;
	int						fd;
	char					redir;
	char					is_quotes;
	int						error;
	char					*ambigous;
	int						pipe_fd[2];
	struct	s_list_file		*next;
	struct	s_list_file		*previous;
}					t_list_file;

typedef	struct		s_list
{
	char			**cmds;
	t_list_file		*file;
	pid_t	pid;
	int		is_fork;
//	char			*cmds_type;
//	int				*fd;
//	char	*path;
	int		is_piped;
	char	type;//pipe
	int		pipe[2]; // SAME QUE SUR list_file
//  POUR GERER LES AMBIGOUS REDIRECTION DANS CHAQUE CMD (UN MESSAGE PAR COMMANDE)
//  NE PAS OUBLIER LE CAS OU L EXPANSION EST IGNORE DANS LE CAS D UN HEREDOC et si precence
//  de ""ou'' 
	int					error;
	struct	s_list		*previous;
	struct	s_list		*next;
}					t_list;

typedef	struct		s_env
{
	t_list			*cmds;
	t_list_env		*env;
	char			**split_path;
	// POUR LES ERREURS
	int				error;
	// Unexpexted token
	char			none_ex;
	// POUR SIMPLIFIER LE PARSING
	// UN INT WORD (correspondra au mot dans cmds)
	// UN INT I (correspondra a l index qu on utilisera pour parcourir line[i])
	// UN INT CMDS(dira si correspond a une cmds)
	char			*line; 
	int				last_ret;
	int				word;
	int				empty;
	char			last_type;
	int				i;
	int				is_cmds;
}					t_env;


void		find_var_and_strlen_cmds(char *line, t_env *env, int *count);
int			count_redir(char *line, int j);
void		restart_t_list_file(t_list_file		**file);

// pwd
char		*find_value(t_list_env *env, char *key);

//	clear
void		clear_cmds(t_list **cmds);
void		clear_tab(char ***s);
void		clear_env(t_env *env);
void		clear_list_env(t_list_env **env);

// BUILT_IN
int		cd(t_list *cmds, t_list_env *env, bool fork);
int		echo_build(t_list *cmds);
int		unset(t_list *cmds, t_env *env);
int		export_build(t_list *cmds, t_env *env);
int		pwd(t_env *env);
int		exit_build(t_env *env, t_list *cmds, bool fork);

// PARSING
// COUNT WORD
void		skip_space(char *line, int *j);
int			is_quotes(char c, char *line, int i);
int			count_word(char *line, int *i, t_env *env);

// COUNT char
int			count_char(char *line, int i, t_env *env);

// VIEW-CMDS
void		view_cmds(t_list	**cmds);
void		view_t_list_file(t_list_file	**read);
int			find_var_and_strlen(char *line, int *j, t_env *env);

// CONVERT ENV
void		get_splitted_path(t_env *env);
void		ft_dup_env(t_list_env **env, char *var, char **val);
int			ft_convert_env(t_list_env **env, char ***stock);
void		view_tab(char **tab);
int			ft_strlen_env(t_list_env	**list, char *var);
int			view_list_env(t_list_env	**l);
void		list_start_env(t_list_env **list);
int			add_arg(t_list_env	*list, char *var, char *val);

// ENV
void		stock_env(t_env *env, char **envp);
int			ft_len_env(t_list_env **env);
t_list_env	*sub_add_arg_2(char *key, char *value);
int			add_arg_2(t_list_env *env, char *key, char *value);
int			env_manager(char *key, char *value, t_list_env *env);
// BON PROBLEME
//void		add_list_env(t_list_env **env, char *var, char *val);
void		change_value(t_list_env **env, char *var, char *val);
// CMDS
void		add_cmds(t_list	**cmds);

// UTILS
void		ft_strcpy(char **s, char *copy);
int			ft_strcmp(char *s, char *sbis);
int			ft_second(char c, char	*s, int i);
int			is_alphanum(char c);
char		ft_find(char c, char *s);
int			ft_strlen(char *s);
int			ft_strlen_char(char *s, char c);
char		*ft_strdup(char *copy);
char		*ft_strdup_char(char *copy, char c);
char		*ft_strdup_char_after(char *copy, char c);
int			add_list_env(t_list_env	**list, char	*var, char *val, int i);
int			ft_find_env(t_list_env **env, char *var);
void		ft_putstr_fd(char const *s, int fd);
int			cmds_length(t_list *cmds);
void		ft_putstr_fd(char const *s, int fd);
int			ft_strcmp_cd(const char *s1, const char *s2);
char		*ft_itoa(int n);
// exec
void		exec_cmds_old(t_env *env);
int			exec_cmds(t_env *env);
int			wait_execution(t_list *cmds, t_env *env);
int			dup_pipes(t_list *cmd);

//Signals
int			define_signals(void);
int			handle_signals_bis(void);


//
int			handle_signals(void);
void		ctrl_c(int sig);
void		ctrl_slash(int sig);
void		ignore(int sig);
int			setup_signals(void);
int			inhibit_signals(int id);

// redirection
int			exec_other(t_list *c, t_env *env);
int			ft_redirection(t_list_file *file, t_env *env, int is_fork);
int			find_exec_path(char **path, t_env *env);
void		list_cmds_restart(t_list	**cmds);
char		*ft_strnewcat(char *first, char *second);
char		*get_value_from_key(char *str, int *i);
char		*ft_expansion(char *str, t_env *env);

//fancy
void   		 ascii_art(void);

//utils2
int			is_builtin(char *s);
bool		is_piped(t_list *elem);
bool		is_redirected(t_list *elem);
void		*ft_memcpy(void *dest, const void *src, size_t size);
int			list_env_len(t_list_env *env);

//env2.c 
char		**ft_env_string_tab(t_env *env);
int			exec_build(t_list	*a, t_env *env);
int			ft_strcmp_1(char *s1, char *s2);

//utils3.c 
char		*ft_strnewcat(char *first, char *second);
//char 		*ft_expansion(char *str, t_env *env, int i, char *read);
char		*get_value_from_key(char *str, int *i);

int			count_word_before_redir(char	*line, int  i);

void		ft_dup_fd2(t_list_file *cmd);

//PARSING
void			is_in_word_not_quotes(char *line, int *j, int *redir, int *pipe);
void			spe_case(char *line, int *j, int *pipe, int *redir);

//EXECUTION
int			exec_not_build_not_pipe(t_list	*cmd, t_env *env);
int			exec_build_not_pipe(t_list	*cmd, t_env *env);
void		exec_pipe(t_list *cmd, t_env *env, int is_piped);

//ERROR
void		exit_fatal(int	n, t_env *env);
void		error_exec(int	n, t_env *env);
int			error_redirection(t_list_file	*f, char y);
int			error_unexpected(int n, t_env *env);

// LAURANNE NORM

void	restart_t_list_file(t_list_file **file);
void	add_list_file(t_list_file **file, int i, char c);
int		find_var_and_strlen(char *line, int *j, t_env *env);
void	skip_quotes(char *line, int *j);
int		nb_redir(char *line, int h);
void	ajout_cmds(t_env *env, char *line, int *i);
void	is_pipe(t_env *env, char *line);
int		is_redir(char *line, int *i, t_env *env);
void	is_quotes_cmds(char *line, int *count, t_env *env);
int			next_is_word(char *line, int i);
int		is_redir_pipe_space(char c);
void	find_var_and_strlen_2(char *stock, t_env *env, int *count);
void	find_var_and_strlen_cmds(char *line, t_env *env, int *count);
int	is_only_space(char *line);
int	is_quotes(char c, char *line, int i);
void	not_quotes_cmds(char *line, t_env *env, int *count, int *out);
int	save_ambigous(char **stock, char *line, int i);
int	is_only_var(char *line, int i);
void	parse_word_heredoc_0(char *line, int *i, t_env *env);
void	parse_word_heredoc(char *line, int *i, t_env *env, int *count);
void	is_word_cmds_0(char *line, int *i, t_env *env);
void	is_word_cmds_1(char	*line, int	*count, int	*i, t_env *env);
void	is_word_cmds(char *line, int *i, t_env *env);
int	next_is_word(char *line, int i);
void	init_parse_line(t_env *env, char *line);


void	make_concat(t_env *env, char *s);

void	export_simple(char	*s, t_env *env);
void	list_sort(t_list_env *env);


void	add_before(t_list_env **list, char *var, char *val);
void	concat_value(t_list_env **env, char *var, char *val);

int	is_only_alpha_num(char	*var);

void	add_after(t_list_env **list, char *var, char *val);

int		redir_output_simple(t_list_file	*f);
int		redir_input_simple(t_list_file	*f, t_env *env, int is_fork);

char	*ft_strndup(char *str, int n);

char	*ft_get_val(char *var, t_env *env);

int	ft_count_char(char *line, t_env *env);


#endif
