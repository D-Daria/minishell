/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:08:10 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/02 17:35:50 by mrhyhorn         ###   ########.fr       */
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

#ifndef NULL
# define NULL		(void *)0
#endif

/*TOKENS IDs*/
# define L1_REDIRECT 1
# define L2_HEREDOC 2
# define R1_REDIRECT 3
# define R2_REDIRECT 4
# define PIPE 5
# define WORD 6

typedef struct stat	t_stat;
typedef struct termios t_termios;

typedef struct s_parser {
	char	***cmds;
	char	**commands;
	char	**paths;
	char	**cmd_path;
	size_t	n_commands;
}			t_parser;

typedef struct s_data {
	char		*last_user_cmd;
	char		**envp;
	int			stopshell;
	char		*expand_dollar;
	int			status;
	char		*builtins[8];
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

typedef void (*ptr_fn_builtin)(t_data *data, t_list *cmd);

void	ft_sigint_handler(int signum);

/*utils.c*/
void	ft_error_exit(const char *error);
int		ft_throw_system_error(const char *str);
size_t	ft_split_len(char **str);

/*free_utils.c*/
void	ft_free_commands(t_list **cmd_head);
void	ft_free_redirs(t_list **redir_head);
void	ft_free_data_ptr(t_data *data_ptr);
void	ft_free_tokenlist(t_list **list);

/*debug.c*/ //tests
void	debug_print_double_arr(char **arr);
void	ft_print_list_of_tokens(t_data *data);
void	ft_print_envplist(t_data *data);
void	debug_print_commands_list(t_data *data);
void 	debug_print_redirections(t_list *redirs);

//read_user_cmd.c
void    ft_read_user_cmd(t_data *data_ptr);

/*parser.c*/
void		ft_parser(t_data *data_ptr);
t_list		*ft_calloc_new_token(void);
void		ft_parse_quotes(t_data *data, size_t *i, size_t *st);

/*commands.c*/
void		ft_commands(t_data *data);

/*commands_utils.c*/
t_command	*ft_create_command(char **cmd_args);
t_list		*ft_new_cmd_lst(char *cmd_path, char **cmd_args, int id, int num);
void		ft_get_cmd(t_list ****token, char ***cmd);

/*paths.c*/
char		*ft_access_paths(t_parser *parser, char *cmd);
void		ft_get_paths(t_data *data, t_parser *parser);

//env_variables_utils.c
char    	*ft_getenv(t_data *data, char *var);
void		ft_adding_var_to_sortlist_if_flag(t_data *data, char *s_new);
void   		ft_adding_var_to_envplist_if_flag(t_data *data, char *s_new);

/*builtin_export.c*/
void		ft_export(t_data *data, t_list *cmd);

/*builtin_unset.c*/
void		ft_unset(t_data *data, t_list *cmd);
void		ft_get_length_var(char *cmd, size_t *length);

/*builtins_utils.c*/
void		ft_set_builtins(t_data *parser);
int			ft_processing_builtin(t_data *data, t_list *cmd);
void		ft_start_builtin(t_data *data, t_list *cmd, int ret);

/*builtins.c*/
void		ft_echo(t_data *data, t_list *cmd);
void		ft_cd(t_data *data, t_list *cmd);
void		ft_pwd(t_data *data, t_list *cmd);
void		ft_env(t_data *data, t_list *cmd);
void		ft_exit(t_data *data, t_list *cmd);

/*execution.c*/
void		ft_execute(t_data *data);
void		ft_execve(t_data *data, t_list *cmd);
void		ft_get_status(t_data *data);

/*execution_utils.c*/
void		ft_close_pipes(t_list *current);
void		ft_dup(t_list **cmd, t_list **prev);
void		ft_get_status(t_data *data);
void		ft_wait_children(t_data *data);

/*exection_errors.c*/
void		ft_perror(t_list *cmd);
int			ft_close_all(t_data *data);
void		ft_perror_redir(t_data *data, t_list *redir);
void		ft_file_error(t_data *data, char *file, int process);
void		ft_token_error(t_data *data, int id, int process);

/*redirection.c*/
void		ft_redirect(t_list *cmd, t_data *data);
void		ft_process_redirs(t_data *data);
t_list		*ft_new_redir_lst(char *file, int id, int num);

/*redirection_utils.c*/
int			ft_open_files(t_data *data, t_list *redir, int id, int process);
void		ft_process_redirs(t_data *data);

#endif