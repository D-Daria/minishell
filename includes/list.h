/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:54:01 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/05 22:43:16 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

typedef struct s_list t_list;
typedef struct s_redir t_redir;

typedef struct s_redir {
	int		num;
	int		id;
	char	*file;
	int		fd;
}			t_redir;

typedef struct s_command {
	int		cmd_num;
	char	**cmd;		/* команда с аргументами ("ls" "-l" "-a" "-p") */
	char	*cmd_path;	/* путь к команде ("/bin/ls") */
	int		cmd_id;		/* id токена (redirect; here_doc) */
	int		pipe_fd[2];
	int		is_redir;
	pid_t	pid;
	t_list	*heredoc;
	t_list	*redir_in;
	t_list	*redir_out;
}			t_command;

typedef struct s_content_for_list {
	char	*token;
	int		token_id;
	size_t	token_number;
}			t_content_for_list;

typedef struct s_list
{
	t_redir				*redir_data;
	t_command			*cmd_data;
	t_content_for_list	*content;
	char				*envp_str;
	struct s_list		*next;
}						t_list;

#endif