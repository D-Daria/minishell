/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:08:10 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:38:45 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "./list.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>
# include <stddef.h>
# include <fcntl.h>
# include <dirent.h> //opendir closedir
# include <termios.h>
# include <stdbool.h>

# define PROMPT	"[minishell]-> "

/*COLORS*/
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define BLUE	"\033[0;34m"
# define YELLOW	"\033[0;33m"
# define BREAK	"\033[0m"

# define SUCCESS	0
# define ERROR		-1

/*TOKENS IDs*/
# define L1_REDIRECT 1
# define L2_HEREDOC 2
# define R1_REDIRECT 3
# define R2_REDIRECT 4
# define PIPE 5
# define WORD 6

typedef struct s_data		t_data;
extern int					g_status;
typedef struct sigaction	t_sig;
typedef struct stat			t_stat;
typedef struct termios		t_termios;
typedef void				(*t_ptr_fn_builtin)(t_data *data, t_list *cmd);

typedef struct s_parser {
	char	***cmds;
	char	**commands;
	char	**paths;
	char	**cmd_path;
	size_t	n_commands;
}			t_parser;

typedef struct s_data {
	char		*oldpwd;
	bool		oldpwd_flag;
	char		*pwd;
	bool		pwd_flag;
	char		*last_user_cmd;
	char		**current_arr_env_vars;
	size_t		amount_env_vars;
	char		*expand_dollar;
	bool		exp_dol_flag;
	char		*builtins[8];
	int			exit_status;
	size_t		pipes_number;
	size_t		cmds_number;
	t_parser	*parser_ptr;
	t_list		*redirs;
	t_list		*tokens;
	t_list		*last_token;
	t_list		*commands;
	t_list		*envplist;
	t_list		*sorted_envplist;
	bool		add_new_var_envplist;
	bool		add_new_var_sortlist;
}				t_data;

/*main_init.c*/
void		ft_init(t_data *data_ptr, char **envp);

/*utils.c*/
void		ft_error_exit(const char *error);
int			ft_throw_system_error(const char *str);
size_t		ft_split_len(char **str);
bool		ft_check_in_change_envplist(t_data *data, char *new_var, char ch);

/*free_utils.c*/
void		ft_free_commands(t_list **cmd_head);
void		ft_free_redirs(t_list **redir_head);
void		ft_free_data_ptr(t_data *data_ptr);
void		ft_free_tokenlist(t_list **list);

/*read_user_cmd.c*/
void		ft_read_user_cmd(t_data *data_ptr);

/*parser.c*/
void		ft_parser(t_data *data_ptr);

/*parser_quotes.c*/
void		ft_parse_quotes(t_data *data, size_t *i, size_t *st);

/*parser_dollar.c*/
void		ft_add_expand_dollar_to_word(t_data *data, char *str, size_t *i);

/*parser_utils.c*/
void		ft_add_this_str_to_word(t_data *data, size_t *i, size_t *st);
void		ft_create_token(t_data *data, int id, size_t *i, size_t *st);
void		ft_create_token_pipe_redir_heredoc(t_data *data, size_t *i, \
																size_t *st);

/*signals.c*/
int			ft_signal(int signum, void f(int));
void		ft_sigint_handler(int signum);
void		ft_sigint_parent(int signum);
void		ft_signals_child(void);

/*commands.c*/
void		ft_commands(t_data *data);

/*commands_utils.c*/
t_command	*ft_create_command(char **cmd_args);
t_list		*ft_new_cmd_lst(char *cmd_path, char **cmd_args, int id, int num);
void		ft_get_cmd(t_list ****token, char ***cmd);
void		ft_fill_command(t_data *data, t_list ***token, int id, int num);

/*paths.c*/
char		*ft_access_paths(t_parser *parser, char *cmd);
void		ft_get_paths(t_data *data, t_parser *parser);

/*env_vars_utils.c*/
char		*ft_getenv(t_data *data, char *var);
void		ft_change_arr_env_vars(t_data *data);
void		ft_free_arr_env_vars(char **arr);
char		*ft_get_name_env_var(char *cmd);
void		ft_get_length_var(char *cmd, size_t *length);

/*env_vars_pwd_oldpwd.c*/
void		ft_exp_pwd(t_data *data);
void		ft_exp_oldpwd(t_data *data);
void		ft_change_data_oldpwd(t_data *data, char *new_oldpwd);
void		ft_change_data_pwd(t_data *data, char *new_pwd);
void		ft_change_data_pwd_or_oldpwd(t_data *data, char *cmd);

/*builtin_unset.c*/
void		ft_unset(t_data *data, t_list *cmd);
void		ft_get_length_var(char *cmd, size_t *length);

/*builtins_utils.c*/
void		ft_set_builtins(t_data *parser);
int			ft_processing_builtin(t_data *data, t_list *cmd);
void		ft_start_builtin(t_data **data, t_list *cmd, int ret);
void		ft_single_builtin(t_data *data, t_list *cmd, int index);

/*builtins.c*/
void		ft_echo(t_data *data, t_list *cmd);
void		ft_cd(t_data *data, t_list *cmd);
void		ft_pwd(t_data *data, t_list *cmd);
void		ft_env(t_data *data, t_list *cmd);
void		ft_exit(t_data *data, t_list *cmd);

/*builtin_exit.c*/
void		ft_exit_process(t_data *data, t_list *cmd);

/*builtin_cd.c*/
void		ft_cd_process(t_data *data, t_list *cmd);

/*builtin_echo.c*/
void		ft_echo_process(t_data *data, t_list *cmd);

/*builtin_export.c*/
void		ft_export(t_data *data, t_list *cmd);
void		ft_processing_exported_vars(t_data *data, char ***tmp_cmd);

/*builtin_export_listshandling.c*/
void		ft_change_envplist_if_var_found(char *var, size_t l, t_data *data);
void		ft_change_sortlist_if_var_found(char *var, size_t l, t_data *data);
void		ft_adding_var_to_sortlist_if_flag(t_data *data, char *s_new);
void		ft_adding_var_to_envplist_if_flag(t_data *data, char *s_new);

/*builtin_export_utils.c*/
void		ft_init_change_func(t_data *d, t_list **l, char **old, bool env);
void		ft_create_lstnew(t_list **l_new, char *str_new);
bool		ft_not_change(t_data *d, bool env, char *old, char *new);
void		ft_change_var(t_list **lst, char *new, t_data *d, char name_lst);
bool		ft_new_var_is_empty(char last_ch, t_data *d, char *old, char *new);

/*builtin_unset.c*/
void		ft_unset(t_data *data, t_list *cmd);
void		ft_delete_if_found_in_envplist(char **tmp_cmd, t_data *data);

/*execution.c*/
void		ft_execute(t_data *data);
void		ft_execve(t_data *data, t_list *cmd);

/*execution_dup.c*/
void		ft_dup(t_list **cmd, t_list **prev);
void		ft_backup_dup(int *tmp_fd_in, int *tmp_fd_out, int type);

/*execution_utils.c*/
void		ft_close_pipes(t_list *current);
int			ft_close_all(t_data *data);
void		ft_get_status(t_data *data, t_list *cmd);
void		ft_wait_children(t_data *data);

/*exection_errors.c*/
void		ft_perror(t_data *data, t_list *cmd);
void		ft_perror_redir(t_data *data, t_list *redir);
void		ft_file_error(t_data *data, char *file, int process);
void		ft_token_error(t_data *data, int id, int process);

/*redirection.c*/
void		ft_redirect(t_list *cmd, t_data *data, int is_process);
t_list		*ft_new_redir_lst(char *file, int id, int num);

/*redirection_open.c*/
void		ft_open_redirs(t_data *data);
void		ft_open_heredoc(t_data *data);
void		ft_set_cmd_redirs(t_data *data);
void		ft_set_redir(t_data *data, t_list **cmd_redir, int cmd_num, \
																int type);

/*redirection_utils.c*/
int			ft_open_files(t_data *data, t_list *redir, int id, int process);

/*heredoc.c*/
int			ft_process_heredoc(t_data *data, t_list *redir);

#endif