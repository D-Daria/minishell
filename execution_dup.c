/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 19:15:32 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/11 19:18:19 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dup(t_list **cmd, t_list **prev)
{
	if ((*prev))
	{
		dup2((*prev)->cmd_data->pipe_fd[0], STDIN_FILENO);
		close((*prev)->cmd_data->pipe_fd[0]);
		close((*prev)->cmd_data->pipe_fd[1]);
	}
	if ((*cmd)->next)
	{
		close((*cmd)->cmd_data->pipe_fd[0]);
		dup2((*cmd)->cmd_data->pipe_fd[1], STDOUT_FILENO);
		close((*cmd)->cmd_data->pipe_fd[1]);
	}
	if (!(*prev) && !(*cmd)->next)
	{
		dup2((*cmd)->cmd_data->pipe_fd[0], STDIN_FILENO);
		close((*cmd)->cmd_data->pipe_fd[0]);
		close((*cmd)->cmd_data->pipe_fd[1]);
	}
}

void	ft_backup_dup(int *tmp_fd_in, int *tmp_fd_out, int type)
{
	if (type == 'b')
	{
		*tmp_fd_in = dup(STDIN_FILENO);
		*tmp_fd_out = dup(STDOUT_FILENO);
	}
	else if (type == 'r')
	{
		if (*tmp_fd_in >= 0)
		{
			dup2(*tmp_fd_in, STDIN_FILENO);
			close(*tmp_fd_in);
		}
		if (*tmp_fd_out >= 0)
		{
			dup2(*tmp_fd_out, STDOUT_FILENO);
			close(*tmp_fd_out);
		}
	}
}
