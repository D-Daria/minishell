/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 16:16:16 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/24 23:44:15 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static void	ft_execute_child(t_data *data, t_list *cmd)
{
	t_list	*redir;
	int		id;

	// dup2(data->fd_in, STDIN_FILENO); // read_in
	// close(data->fd_in);
	if (!cmd->next)
		ft_execute_single_cmd(data, cmd);
	if (cmd->next)
		dup2(data->fd_pipe[1], STDOUT_FILENO);
	redir = NULL;
	// if (data->redirs)
	// {
		printf("is redirect\n");
		redir = data->redirs;
		while (redir)
		{
			printf("several redirects\n");
			id = redir->redir_data->id;
			if (ft_check_files(data, redir, id, 1) == -1)
				ft_perror(data, cmd, redir);
			ft_redirect(cmd, data);
			redir = redir->next;
		}
	// }
	close(data->fd_pipe[0]);
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	else
		ft_perror(data, cmd, redir);
	exit(0);	
}
*/

void	ft_execve(t_data *data, t_list *cmd)
{
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	ft_perror(cmd);
	exit(0);
}

void	ft_execute_single_cmd(t_data *data, t_list *cmd)
{
	pid_t	pid;
	
	printf("execute single\n");
	pid = fork();
	if (pid < 0)
		exit(ft_throw_system_error("fork"));
	else if (pid == 0)
	{
		printf("parent pid: %d - child pid : %d\n", getppid(), getpid());
		signal(SIGINT, &ft_sigint_handler);//ctr+C передаю в обработчик
		if (cmd->cmd_data->cmd_redir_in || cmd->cmd_data->cmd_redir_out)
			ft_redirect(cmd, data);
		ft_execve(data, cmd);
	}
	else
	{
		waitpid(pid, &data->status, 0);
		printf("execute_single status: %d\n", WEXITSTATUS(data->status));
	}
}

/*	TODO
	учесть сигналы в дочерних процессах
*/

void	ft_execute_child(t_data *data, t_list *cmd)
{
	signal(SIGINT, &ft_sigint_handler);//ctr+C передаю в обработчик
	dup2(data->fd_in, STDIN_FILENO); // read_in
	// close(data->fd_in);
	if (cmd->cmd_data->cmd_redir_in || cmd->cmd_data->cmd_redir_out)
	{
		printf("cmd: %s\n", cmd->cmd_data->cmd[0]);
		ft_redirect(cmd, data);
	}
	else if (cmd->next)
		dup2(data->fd_pipe[1], STDOUT_FILENO);
	close(data->fd_pipe[0]);
	ft_execve(data, cmd);
}

int	ft_pipe(t_data *data)
{
	t_list	*cmd;
	pid_t	pid;

	data->fd_pipe[0] = -1;
	data->fd_pipe[1] = -1;
	data->fd_in = STDIN_FILENO;
	cmd = data->commands;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(data->fd_pipe) < 0)
				return (ft_throw_system_error("pipe"));
		}
		// if builtin -> ft_execute_builtin
		// else
		pid = fork();
		if (pid < 0)
			return (ft_throw_system_error("fork"));
		else if (pid == 0)
		{
			printf("execute simple multiple command: %s\n", cmd->cmd_data->cmd_path);
			printf("parent pid: %d - child pid : %d\n", getppid(), getpid());
			ft_execute_child(data, cmd); // simple command
		}
		else
		{
			waitpid(pid, &data->status, 0);
			printf("status: %d\n", WEXITSTATUS(data->status));
			data->fd_in = data->fd_pipe[0];
			close(data->fd_pipe[1]);
			if (!cmd->next)
				close(data->fd_pipe[0]);
			cmd = cmd->next;
		}
	}
	return (0);
}

//попробовать открыть файлы заранее
/* need to close all files!!! */

void	ft_execute(t_data *data)
{
	t_list	*cmd;

	cmd = data->commands;
	if (data->redirs)
		ft_process_redirs(data);
	debug_print_commands_list(data);
	if (cmd && data->pipes_number <= 0)
		ft_execute_single_cmd(data, cmd);
	else
		ft_pipe(data);
	ft_close_all(data, NULL);
	printf("status: %d\n", data->status);
}
