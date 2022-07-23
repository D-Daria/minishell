/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:54:01 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/23 17:03:24 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

typedef struct s_list t_list;
typedef struct s_redir t_redir;

typedef struct s_redir {
	int				id;
	char			*file;
	int				num;
}					t_redir;

typedef struct s_command {
	int		num;
	char	**cmd;			/* команда с аргументами ("ls" "-l" "-a" "-p") */
	char	*cmd_path;	/* путь к команде ("/bin/ls") или имя файла [0]: file */
	int		token_id;	/* id токена (redirect; here_doc) */
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
	struct s_list		*next;
}						t_list;

#endif