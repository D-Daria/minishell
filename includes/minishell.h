/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:08:10 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/29 13:07:16 by mrhyhorn         ###   ########.fr       */
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

typedef struct s_parser {
	char	***cmds;
	char	**commands;
	char	**paths;
	char	**cmd_path;
	size_t	n_commands;
}			t_parser; // заменить на t_command ?

typedef struct s_data {
	char		*last_user_cmd;
	char		**envp;
	char		**path_by_launch;
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
}				t_data;

void	ft_sigint_handler(int signum);

/*utils.c*/
void	ft_error_exit(const char *error);
void	ft_free_3darray(char ***arr);
int		ft_throw_system_error(const char *str);
char	**ft_strrev_split(char *str, char c);
size_t	ft_split_len(char **str);

/*free_utils.c*/
void	ft_free_commands(t_list **cmd_head);
void	ft_free_redirs(t_list **redir_head);
void	ft_free_data_ptr(t_data *data_ptr);

/*debug.c*/ //tests
void	debug_print_double_arr(char **arr);
void	ft_print_list_of_tokens(t_data *data);
void	debug_print_commands_list(t_data *data);
void 	debug_print_redirections(t_list *redirs);

//read_user_cmd.c
void    ft_read_user_cmd(t_data *data_ptr);

/*ft_reading_parser.c*/
void		ft_parser(t_data *data_ptr);

// now
t_list		*ft_calloc_new_token(void);
void		ft_parse_quotes(t_data *data, size_t *i, size_t *st);
void		ft_free_list(t_list **list);

/*commands.c*/
void		ft_commands(t_data *data);

/*commands_utils.c*/
void		ft_get_paths(t_parser *parser);
t_command	*ft_create_command(char **cmd_args);
t_list		*ft_new_cmd_lst(char *cmd_path, char **cmd_args, int id, int num);
char		*ft_access_paths(t_parser *parser, char *cmd);
void		ft_get_cmd(t_list ****token, char ***cmd);

/*builtins.c*/
void		ft_set_builtins(t_data *parser);
int			ft_is_builtin(t_data *data, char *cmd);

/*execution.c*/
void		ft_execute(t_data *data);

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