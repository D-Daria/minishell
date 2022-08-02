/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 17:31:25 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/02 20:22:16 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_get_status(t_data *data)
{
	printf("exit status: %d\n", WEXITSTATUS(data->status));
	printf("stop status: %d\n", WSTOPSIG(data->status));
	printf("if signaled: %d\n", WIFSIGNALED(data->status));
	if (WIFSIGNALED(data->status))
		data->status = 128 + SIGINT;
	else
		data->status = WEXITSTATUS(data->status);
	printf("data->status: %d\n", data->status);
}

void	ft_wait_children(t_data *data)
{
	t_list	*cmd;

	cmd = data->commands;
	while (cmd)
	{
		waitpid(0, &data->status, 0);
		ft_get_status(data);
		cmd = cmd->next;
	}
	ft_close_all(data);
}

void	ft_close_pipes(t_list *current)
{
	close(current->cmd_data->pipe_fd[0]);
	close(current->cmd_data->pipe_fd[1]);
}

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
	if (!(*prev) && !(*cmd)->next) /* в случае > file | wc -l */
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