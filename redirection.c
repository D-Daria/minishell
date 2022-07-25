/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:08:37 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/25 15:21:42 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_new_redir_lst(char *file, int id, int num)
{
	t_list	*redir_lst;
	t_redir *redir;

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

void	ft_process_redirs(t_data *data)
{
	t_list	*prev;
	t_list	*redir;
	int		id;

	redir = data->redirs;
	prev = NULL;
	while (redir)
	{
		id = redir->redir_data->id;
		if (redir->redir_data->file == NULL)
		{
			ft_perror_redir(data, redir);
			break;
		}
		redir->redir_data->fd = ft_open_files(data, redir, id, 0);
		if (redir->redir_data->fd < 0)
		{
			ft_perror_redir(data, redir);
			break;
		}
		redir = redir->next;
	}
}

void	ft_redirect(t_list *cmd, t_data *data)
{
	t_list	*redir_in;
	t_list	*redir_out;
	int id;

	if (!cmd)
		return ;
	redir_in = NULL;
	redir_out = NULL;
	id = 0;
	// printf("ft_redirect\n");
	if (cmd->cmd_data->cmd_redir_in)
	{
		redir_in = cmd->cmd_data->cmd_redir_in;
		if (redir_in->redir_data->fd < 0)
			ft_file_error(data, redir_in->redir_data->file, 1);
		id = redir_in->redir_data->id;
		if (id == L1_REDIRECT)
		{
			dup2(redir_in->redir_data->fd, STDIN_FILENO);
			close(redir_in->redir_data->fd);
		}
	}
	if (cmd->cmd_data->cmd_redir_out)
	{
		redir_out = cmd->cmd_data->cmd_redir_out;
		if (redir_out->redir_data->fd < 0)
			ft_file_error(data, redir_out->redir_data->file, 1);
		id = redir_out->redir_data->id;
		if (id == R1_REDIRECT || id == R2_REDIRECT)
		{
			dup2(redir_out->redir_data->fd, STDOUT_FILENO);
			close(redir_out->redir_data->fd);
		}
	}
}
