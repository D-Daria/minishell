/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:54:01 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/20 16:44:18 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

typedef struct s_command {
	char	**cmd; 		/* команда с аргументами ("ls" "-l" "-a" "-p") или имя файла [0]: file */
	char	*cmd_path;	/* путь к команде ("/bin/ls") */
	int		token_id;	/* id токена (redirect; here_doc) */
}			t_command;

typedef struct s_content_for_list {
	char	*token;
	int		token_id;
	size_t	token_number;
}			t_content_for_list;

typedef struct s_list
{
	t_command			*cmd_data;
	t_content_for_list	*content;
	struct s_list		*next;
}						t_list;

#endif