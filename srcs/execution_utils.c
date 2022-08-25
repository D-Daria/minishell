/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 17:31:25 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:14:54 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_kill_all(t_list *cmd, int signal)
{
	t_list	*cmd_tmp;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		kill(cmd->cmd_data->pid, signal);
		cmd_tmp = cmd_tmp->next;
	}
}

void	ft_get_status(t_data *data, t_list *cmd)
{
	(void)data;
	if (WIFSIGNALED(g_status))
	{
		if (WTERMSIG(g_status) == SIGQUIT)
		{
			if (!cmd->next)
				printf("Quit: %d\n", WTERMSIG(g_status));
			ft_kill_all(cmd, SIGQUIT);
		}
		if (WTERMSIG(g_status) == SIGINT)
			ft_kill_all(cmd, SIGINT);
		g_status = 128 + WTERMSIG(g_status);
	}
	else
		g_status = WEXITSTATUS(g_status);
}

int	ft_close_all(t_data *data)
{
	t_list	*redir;

	redir = data->redirs;
	while (redir)
	{
		if (redir->redir_data->fd > 0)
			close(redir->redir_data->fd);
		redir = redir->next;
	}
	if (access("here_doc", F_OK) == 0)
		unlink("here_doc");
	return (1);
}

void	ft_wait_children(t_data *data)
{
	t_list	*cmd;

	cmd = data->commands;
	while (cmd)
	{
		waitpid(cmd->cmd_data->pid, &g_status, 0);
		ft_get_status(data, cmd);
		cmd = cmd->next;
	}
	ft_close_all(data);
}

void	ft_close_pipes(t_list *current)
{
	close(current->cmd_data->pipe_fd[0]);
	close(current->cmd_data->pipe_fd[1]);
}
