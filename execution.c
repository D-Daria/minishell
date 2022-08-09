/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 15:10:41 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/09 16:06:24 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execve(t_data *data, t_list *cmd)
{
	if (cmd->cmd_data->cmd_path == NULL)
	{
		ft_perror(data, cmd);
		return ;
	}
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
	{
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, \
		data->current_arr_env_vars);
		ft_perror(data, cmd);
	}
	ft_perror(data, cmd);
}

void	ft_execute_single_cmd(t_data *data, t_list *cmd)
{
	int	is_builtin;

	is_builtin = ft_processing_builtin(data, cmd);
	if (is_builtin >= 0)
	{
		ft_single_builtin(data, cmd, is_builtin);
		return;
	}
	cmd->cmd_data->pid = fork();
	if (cmd->cmd_data->pid < 0)
		exit(ft_throw_system_error("fork"));
	else if (cmd->cmd_data->pid == 0)
	{
		ft_signals_child();
		if (cmd->cmd_data->is_redir)
			ft_redirect(cmd, data);
		ft_execve(data, cmd);
		exit(data->status);
	}
	else
	{
		waitpid(cmd->cmd_data->pid, &data->status, 0);
		ft_get_status(data, cmd);
	}
}

void	ft_execute_child(t_data *data, t_list **cmd, t_list **prev)
{
	int	is_builtin;

	ft_signals_child();
	ft_dup(cmd, prev);
	if ((*cmd)->cmd_data->is_redir)
		ft_redirect((*cmd), data);
	is_builtin = ft_processing_builtin(data, (*cmd));
	if (is_builtin >= 0)
		ft_start_builtin(&data, (*cmd), is_builtin);
	else
		ft_execve(data, (*cmd));
	exit(EXIT_SUCCESS);
}

int	ft_pipe(t_data *data, t_list *cmd, t_list *prev)
{
	while (cmd)
	{
		if (pipe(cmd->cmd_data->pipe_fd) < 0)
			return (ft_throw_system_error("pipe"));
		cmd->cmd_data->pid = fork();
		if (cmd->cmd_data->pid < 0)
			return (ft_throw_system_error("fork"));
		else if (cmd->cmd_data->pid == 0)
			ft_execute_child(data, &cmd, &prev);
		else
		{
			if (prev)
				ft_close_pipes(prev);
			if (!cmd->next)
				ft_close_pipes(cmd);
			if (access("here_doc", F_OK) == 0)
				unlink("here_doc");
			prev = cmd;
			cmd = cmd->next;
		}
	}
	ft_wait_children(data);
	return (0);
}

void	ft_execute(t_data *data)
{
	t_list	*cmd;
	t_list	*prev;

	data->status = 0;
	cmd = data->commands;
	prev = NULL;
	if (data->redirs)
		ft_process_redirs(data);
	debug_print_commands_list(data);
	if (data->status > 0)
		return ;
	if (cmd && data->pipes_number > 0)
		ft_pipe(data, cmd, prev);
	else if (cmd)
		ft_execute_single_cmd(data, data->commands);
}
