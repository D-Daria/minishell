/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:29:08 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/23 18:33:39 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_close_all(t_data *data, const char *error)
{
	int	ret;

	ret = 0;
	if (data->fd_in)
		ret = close(data->fd_in);
	if (data->fd_out)
		ret = close(data->fd_out);
	// printf("ret1: %d\n", ret);
	ret = close(data->fd_pipe[0]);
	// printf("ret2: %d\n", ret);
	ret = close(data->fd_pipe[1]);
	// printf("ret3: %d\n", ret);
	// ret = close(fd_read);
	// printf("ret4: %d\n", ret);
	if (error)
		perror(error);
	return (1);
}

void	ft_token_error(t_list *cmd, int id)
{
	char	*token;

	token = NULL;
	printf("token error\n");
	if (cmd->cmd_data->token_id >= L1_REDIRECT &&
		cmd->cmd_data->token_id <= PIPE)
	{
		if (id == L1_REDIRECT)
			token = "'<'";
		else if (id == L2_HEREDOC)
			token = "'<<'";
		else if (id == R1_REDIRECT)
			token = "'>'";
		else if (id == R2_REDIRECT)
			token = "'>>'";
		ft_putstr_fd("syntax error near unexpected token ", STDERR_FILENO);
		ft_putstr_fd(token, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(258);
	}
}

// void	ft_file_error(t_data *data, char *file_name)
// {
// 	ft_putstr_fd(strerror(errno), STDERR_FILENO);
// 	ft_putstr_fd(" : ", STDERR_FILENO);
// 	ft_putstr_fd(file_name, STDERR_FILENO);
// 	ft_putstr_fd("\n", STDERR_FILENO);
// 	ft_close_all(data, *fd_read, NULL);
// 	exit(1);
// }

void	ft_perror(t_list *cmd)
{
	char	*path;
	char	**cmd_args;

	printf("perror\n");
	cmd_args = cmd->cmd_data->cmd;
	path = cmd->cmd_data->cmd_path; /* путь к исполняемой команде || файл */
	if (!path && cmd->cmd_data->token_id < WORD)
		ft_token_error(cmd, cmd->cmd_data->token_id);
	if (cmd_args && path && (access(cmd->cmd_data->cmd_path, F_OK) == 0))
	{
		perror(cmd->cmd_data->cmd_path);
		exit(126);
	}
	if (cmd_args && path && (ft_strchr(cmd->cmd_data->cmd_path, '/')))
		perror(cmd->cmd_data->cmd[0]);
	else if (cmd->cmd_data->token_id == WORD)
	{
		ft_putstr_fd(cmd->cmd_data->cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		if (cmd->next == NULL)
			exit(127);
	}
	else
	{
		ft_putstr_fd(cmd->cmd_data->cmd_path, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		if (cmd->next == NULL)
			exit(127);
	}
}