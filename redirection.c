/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:08:37 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/28 21:02:03 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_redir_out(t_data *data, t_list *cmd)
{
	t_list	*redir_out;
	int		id;

	(void)data;
	redir_out = NULL;
	id = 0;
	redir_out = cmd->cmd_data->redir_out;
	if (redir_out->redir_data->fd < 0)
		ft_file_error(data, redir_out->redir_data->file, 1);
	id = redir_out->redir_data->id;
	if (id == R1_REDIRECT || id == R2_REDIRECT)
	{
		dup2(redir_out->redir_data->fd, STDOUT_FILENO);
		close(redir_out->redir_data->fd);
	}
}

void	ft_redir_in(t_data *data, t_list *cmd)
{
	t_list	*redir_in;
	int		id;

	redir_in = NULL;
	id = 0;
	redir_in = cmd->cmd_data->redir_in;
	if (redir_in->redir_data->fd < 0)
		ft_file_error(data, redir_in->redir_data->file, 1);
	id = redir_in->redir_data->id;
	if (id == L1_REDIRECT)
	{
		dup2(redir_in->redir_data->fd, STDIN_FILENO);
		close(redir_in->redir_data->fd);
	}
}

void	ft_heredoc(t_data *data, t_list *cmd)
{
	t_list	*heredoc;
	int		id;

	heredoc = NULL;
	id = 0;
	heredoc = cmd->cmd_data->heredoc;
	if (heredoc->redir_data->fd < 0)
		ft_file_error(data, heredoc->redir_data->file, 1);
	id = heredoc->redir_data->id;
	if (id == L2_HEREDOC)
	{
		dup2(heredoc->redir_data->fd, STDIN_FILENO);
		close(heredoc->redir_data->fd);
	}
	if (access("here_doc", F_OK) == 0)
		unlink("here_doc");
}

void	ft_redir_in_heredoc(t_data *data, t_list *cmd) /*учитываем порядок редиректов*/
{
	t_list	*redir;
	int		cmd_num;
	int		id;

	redir = NULL;
	if (cmd->cmd_data->redir_in)
		redir = cmd->cmd_data->redir_in;
	else if (cmd->cmd_data->heredoc)
		redir = cmd->cmd_data->heredoc;
	cmd_num = cmd->cmd_data->cmd_num;
	while (redir && redir->redir_data->num == cmd_num)
	{
		if (redir->redir_data->fd < 0)
			ft_file_error(data, redir->redir_data->file, 1);
		id = redir->redir_data->id;
		if (id == L1_REDIRECT || id == L2_HEREDOC)
		{
			dup2(redir->redir_data->fd, STDIN_FILENO);
			close(redir->redir_data->fd);
		}
		redir = redir->next;
	}
	if (access("here_doc", F_OK) == 0)
		unlink("here_doc");
}

void	ft_redirect(t_list *cmd, t_data *data)
{	
	if (!cmd)
		return ;
	if (cmd->cmd_data->redir_out)
		ft_redir_out(data, cmd);
	if (cmd->cmd_data->heredoc || cmd->cmd_data->redir_in)
		ft_redir_in_heredoc(data, cmd);
}
