/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:08:37 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/25 18:06:05 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_redir_in(t_data *data, t_list *cmd)
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

static void	ft_redir_out(t_data *data, t_list *cmd)
{
	t_list	*redir_out;
	int		id;

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

void	ft_redirect(t_list *cmd, t_data *data)
{
	if (!cmd)
		return ;
	printf("ft_redirect\n");
	if (cmd->cmd_data->redir_in)
	{
		ft_redir_in(data, cmd);
	}
	if (cmd->cmd_data->redir_out)
	{
		ft_redir_out(data, cmd);
	}
}
