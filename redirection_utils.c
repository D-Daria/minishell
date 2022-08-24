/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:17:47 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:27:46 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_new_redir_lst(char *file, int id, int num)
{
	t_list	*redir_lst;
	t_redir	*redir;

	redir = (t_redir *)ft_calloc(sizeof(t_redir), 1);
	if (!redir)
		return (NULL);
	redir->num = num;
	redir->file = ft_strdup(file);
	redir->id = id;
	redir_lst = (t_list *)ft_calloc(sizeof(t_list), 1);
	if (!redir_lst)
		return (NULL);
	redir_lst->redir_data = redir;
	redir_lst->next = NULL;
	return (redir_lst);
}

int	ft_open_files(t_data *data, t_list *redir, int id, int is_process)
{
	int		fd;
	char	*file_name;

	if (redir->redir_data->file == NULL)
		return (-1);
	file_name = redir->redir_data->file;
	fd = -2;
	if (id == L1_REDIRECT)
		fd = open(file_name, O_RDONLY);
	else if (id == R1_REDIRECT)
		fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else if (id == R2_REDIRECT)
		fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else if (id == L2_HEREDOC && g_status == 0)
		fd = ft_process_heredoc(data, redir);
	if (fd == -1 && is_process)
		ft_file_error(data, file_name, is_process);
	return (fd);
}
