/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:54:01 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 13:44:05 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

typedef struct s_list	t_list;
typedef struct s_redir	t_redir;

typedef struct s_redir {
	int		num;
	int		id;
	char	*file;
	int		fd;
}			t_redir;

typedef struct s_command {
	int		cmd_num;
	char	**cmd;
	char	*cmd_path;
	int		cmd_id;
	int		pipe_fd[2];
	int		is_redir;
	int		is_process;
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