/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 22:29:51 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/28 21:23:32 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execve(t_data *data, t_list *cmd)
{
	int ret;

	ret = 0;
	printf("ft_execve\n");
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
	{
		printf("is accessed\n");
		ret = execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
		printf("ret: %d\n", ret);
		if (ret == -1)
		{
			printf("execve failed");
			ft_perror(cmd);
		}
	}
	ft_perror(cmd);
	exit(0);
}

void	ft_execute_single_cmd(t_data *data, t_list *cmd)
{
	pid_t	pid;
	
	pid = fork();
	if (pid < 0)
		exit(ft_throw_system_error("fork"));
	else if (pid == 0)
	{
		printf("parent pid: %d - child pid : %d\n", getppid(), getpid());
		signal(SIGINT, &ft_sigint_handler);//ctr+C передаю в обработчик
		if (cmd->cmd_data->is_redir)
			ft_redirect(cmd, data);
		ft_execve(data, cmd);
	}
	else
	{
		waitpid(pid, &data->status, 0);
		printf("execute_single_status: %d\n", WEXITSTATUS(data->status));
	}
}

void	ft_execute_child(t_data *data, t_list **cmd, t_list **prev)
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
	if ((*cmd)->cmd_data->is_redir)
		ft_redirect((*cmd), data);
	ft_execve(data, (*cmd));
}

static void	ft_close_pipes(t_list *prev)
{
	close(prev->cmd_data->pipe_fd[0]);
	close(prev->cmd_data->pipe_fd[1]);
}

void	ft_wait_children(t_data *data)
{
	t_list	*cmd;

	cmd = data->commands;
	while (cmd)
	{
		waitpid(0, &data->status, 0);
		printf("status: %d\n", WEXITSTATUS(data->status));
		cmd = cmd->next;
	}
	ft_close_all(data, NULL);
}

int	ft_pipe(t_data *data, t_list *cmd, t_list *prev, int *pid)
{
	while (cmd)
	{
		if (pipe(cmd->cmd_data->pipe_fd) < 0)
			return (ft_throw_system_error("pipe"));
		*pid = fork();
		if (*pid < 0)
			return (ft_throw_system_error("fork"));
		else if (*pid == 0)
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
	int		pid;

	cmd = data->commands;
	prev = NULL;
	if (data->redirs)
		ft_process_redirs(data);
	printf("data->status: %d\n", data->status);
	debug_print_commands_list(data);
	if (cmd && data->pipes_number > 0)
		ft_pipe(data, cmd, prev, &pid);
	else if (cmd)
		ft_execute_single_cmd(data, data->commands);
}
