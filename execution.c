/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 16:16:16 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/23 13:11:18 by mrhyhorn         ###   ########.fr       */
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

// static	void	ft_check_redirs(t_data *data)
// {
// 	t_list	*cmd;
// 	t_list	*prev;
// 	char	*path;
// 	int		id;

// 	cmd = data->commands;
// 	prev = NULL;
// 	while (cmd)
// 	{
// 		id = cmd->cmd_data->token_id;
// 		path = cmd->cmd_data->cmd_path;
// 		if (id == R1_REDIRECT && path)
// 		{
// 			if (prev)
// 				close(data->fd_in);
// 			data->fd_in = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
// 		}
// 		if (id == R2_REDIRECT && path)
// 		{
// 			if (prev)
// 				close(data->fd_in);
// 			data->fd_in = open(cmd->cmd_data->cmd_path, O_WRONLY | O_APPEND | O_CREAT, 0666);
// 		}
// 		if (id == L1_REDIRECT && path)
// 		{
// 			if (prev)
// 				close(data->fd_out);
// 			data->fd_out = open(path, O_RDONLY);
// 		}
// 		prev = cmd;
// 		cmd = cmd->next;
// 	}
// }

static void	ft_execute_child(t_data *data, int *fd_read, t_list *cmd)
{
	// t_list	*tmp;
	// int		id;

	// tmp = NULL;
	// id = 0;
	if (!cmd)
		ft_perror(cmd);
	dup2(*fd_read, STDIN_FILENO); // read_in
	if (cmd->next && cmd->next->cmd_data->token_id == WORD)
		dup2(data->fd_pipe[1], STDOUT_FILENO);
	// tmp = cmd;
	// while (tmp->next && tmp->next->cmd_data->token_id != WORD)
	// {
	// 	printf("next command is redirect\n");
	// 	tmp = tmp->next;
	// 	id = tmp->cmd_data->token_id;
	// 	ft_check_files(data, tmp, fd_read, id);
	// 	ft_redirect(data, fd_read, tmp, id);
	// 	close(data->fd_in);
	// }
	close(data->fd_pipe[0]);
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	ft_perror(cmd);
	if (cmd->next != NULL)
		exit(0);
}

int	ft_pipe(t_data *data, int *fd_read)
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
			return (ft_close_all(data, *fd_read, "perror"));
		else if (pid == 0)
		{
			// if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
			// {
				printf("execute simple command: %s\n", cmd->cmd_data->cmd_path);
				ft_execute_child(data, fd_read, cmd); // simple command
			// }
		}
		else
		{
			waitpid(pid, &data->status, 0);
			printf("status: %d\n", WEXITSTATUS(data->status));
			*fd_read = data->fd_pipe[0];
			close(data->fd_pipe[1]);
			// ft_close_all(data, fd_read, NULL);
		}
		cmd = cmd->next;
	}
	return (0);
}

void	ft_execute(t_data *data)
{
	int	fd_read;

	data->fd_pipe[0] = -1;
	data->fd_pipe[1] = -1;
	debug_print_commands_list(data);
	fd_read = STDIN_FILENO;
	ft_pipe(data, &fd_read);
	ft_close_all(data, fd_read, NULL);
}
