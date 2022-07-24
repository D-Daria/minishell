/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:08:37 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/24 23:33:04 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	printf("ft_redirect\n");
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
			dup2(redir_out->redir_data->fd, STDOUT_FILENO);
		if (redir_out->redir_data->fd)
			close(redir_out->redir_data->fd);
	}
}

int	ft_open_files(t_data *data, t_list *redir, int id, int process)
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
	if (fd == -1 && process)
		ft_file_error(data, file_name, process);
	return (fd);
}