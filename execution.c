/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 16:16:16 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/23 19:02:18 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
static void	ft_execute_child(t_data *data, int *fd_read, t_list *cmd)
{
	if (!cmd)
		ft_perror(cmd);
	if (cmd->cmd_data->token_id == WORD)
	{
		dup2(*fd_read, STDIN_FILENO);
		if (cmd->next && cmd->next->cmd_data->token_id == WORD)
			dup2(data->fd_pipe[1], STDOUT_FILENO);
		else
			
		close(data->fd_pipe[0]);
	}
	if (cmd->cmd_data->token_id != WORD)
	{
		if (cmd->next && cmd->next->cmd_data->token_id == FILE)
		{
			ft_check_files(data, cmd, fd_read);
			ft_redirect(data, fd_read, cmd, cmd->cmd_data->token_id);
		}
	}
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	else
		ft_perror(cmd);
	if (cmd->next != NULL)
		exit(0);
}
*/

void	ft_execute_single_cmd(t_data *data, t_list *cmd, t_list *redirs)
{
	int		id;
	t_list	*crnt_redir;

	printf("execute single\n");
	if (redirs)
	{
		crnt_redir = redirs;
		while (crnt_redir && crnt_redir->redir_data->num == cmd->cmd_data->num)
		{
			printf("not command\n");
			id = crnt_redir->redir_data->id;
			if (ft_check_files(data, crnt_redir, id) == -1)
				ft_perror(cmd);
			ft_redirect(data, id);
			id = close(data->fd_in);
			printf("ret: %d\n", id);
			crnt_redir = crnt_redir->next;
		}
	}
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	ft_perror(cmd);
	exit(0);
}

/*
static void	ft_execute_child(t_data *data, int *fd_read, t_list **cmd)
{
	t_list	*tmp;
	int		id;

	tmp = NULL;
	id = 0;
	if (!(*cmd))
		ft_perror((*cmd));
	if ((*cmd)->next == NULL)
		ft_execute_single_cmd(data, (*cmd), fd_read);
	dup2(*fd_read, STDIN_FILENO); // read_in
	if ((*cmd)->next && (*cmd)->next->cmd_data->token_id == WORD)
		dup2(data->fd_pipe[1], STDOUT_FILENO);
	if ((*cmd)->next && (*cmd)->next->cmd_data->token_id != WORD)
	{
		tmp = (*cmd);
		(*cmd) = (*cmd)->next;
	}
	// printf("cmd_path: %s\n", (*cmd)->cmd_data->cmd_path);
	while ((*cmd) && (*cmd)->cmd_data->token_id != WORD)
	{
		printf("next command is redirect\n");
		id = (*cmd)->cmd_data->token_id;
		ft_check_files(data, (*cmd), fd_read, id);
		ft_redirect(data, fd_read, (*cmd), id);
		close(data->fd_in);
		(*cmd) = (*cmd)->next;
	}
	close(data->fd_pipe[0]);
	if (access(tmp->cmd_data->cmd_path, X_OK) == 0)
		execve(tmp->cmd_data->cmd_path, tmp->cmd_data->cmd, data->envp);
	ft_perror(tmp);
	exit(0);
}
*/

/*	TODO
	учесть сигналы в дочерних процессах
*/


static void	ft_execute_child(t_data *data, t_list *cmd)
{
	// t_list	*redir;
	// int		id;

	if (!cmd)
		ft_perror(cmd);
	if (cmd->next == NULL)
		ft_execute_single_cmd(data, cmd, data->redirs);
	// dup2(data->fd_read, STDIN_FILENO); // read_in
	// if (cmd->next)
	// 	dup2(data->fd_pipe[1], STDOUT_FILENO);
	// if (data->redirs)
	// {
	// 	printf("is redirect\n");
	// 	redir = data->redirs;
	// 	while (redir && redir->redir_data->num == cmd->cmd_data->num)
	// 	{
	// 		printf("several redirects\n");
	// 		id = redir->redir_data->id;
	// 		if (ft_check_files(data, redir, id) == -1)
	// 			ft_perror(cmd);
	// 		ft_redirect(data, id);
	// 		id = close(data->fd_in);
	// 		printf("ret: %d\n", id);
	// 		redir = redir->next;
	// 	}
	// }
	// close(data->fd_in);
	// close(data->fd_pipe[0]);
	// if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
	// 	execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	// ft_perror(cmd);
	// exit(0);
}


int	ft_pipe(t_data *data)
{
	t_list		*cmd;
	pid_t		pid;

	cmd = data->commands;
	while (cmd)
	{
		if (pipe(data->fd_pipe) < 0)
			return (ft_throw_system_error("fork"));
		// if builtin -> ft_execute_builtin
		//else
		pid = fork();
		if (pid < 0)
			return (ft_close_all(data, "perror"));
		else if (pid == 0)
		{
			// if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
			// {
				printf("execute simple command: %s\n", cmd->cmd_data->cmd_path);
				ft_execute_child(data, cmd); // simple command
				// printf("cmd after execution: %s\n", cmd->cmd_data->cmd_path);
				// }
		}
		else
		{
			waitpid(pid, &data->status, 0);
			printf("status: %d\n", WEXITSTATUS(data->status));
			data->fd_read = data->fd_pipe[0];
			close(data->fd_pipe[1]);
			// ft_close_all(data, fd_read, NULL);
		}
		cmd = cmd->next;
	}
	return (0);
}

void	ft_execute(t_data *data)
{
	// int	fd_read;

	data->fd_pipe[0] = -1;
	data->fd_pipe[1] = -1;
	debug_print_commands_list(data);
	data->fd_read = STDIN_FILENO;
	ft_pipe(data);
	ft_close_all(data, NULL);
}
