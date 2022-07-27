/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:29:08 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/24 23:36:04 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_close_all(t_data *data, const char *error)
{
	t_list	*redir;
	// int		ret;

	// ret = 0;
	redir = data->redirs;
	while (redir)
	{
		if (redir->redir_data->fd)
			close(redir->redir_data->fd);
		redir = redir->next;
	}
	if (error)
		perror(error);
	return (1);
}

void	ft_token_error(t_data *data, int id, int process)
{
	char	*token;

	token = NULL;
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
	if (process)
		exit(258);
	else
		data->status = 258;
}

void	ft_file_error(t_data *data, char *file, int process)
{
	int	fd;

	fd = open(file, O_WRONLY);
	if (fd == -1)
	{
		ft_putstr_fd(RED, STDERR_FILENO);
		perror(file);
		ft_putstr_fd(BREAK, STDERR_FILENO);
	}
	else
		close(fd);
	if (process)
		exit(1);
	else
		data->status = 1;
}

void	ft_perror_redir(t_data *data, t_list *redir)
{
	if (!redir)
		return ;
	if (redir->redir_data->file == NULL)
		ft_token_error(data, redir->redir_data->id, 0);
	else if (!data->commands && redir->redir_data->file)
		ft_file_error(data, redir->redir_data->file, 0);
	else
		data->status = 0;
}

void	ft_perror(t_list *cmd)
{
	int		fd;
	DIR		*directory;
	char	*path;

	printf("perror\n");
	path = NULL;
	fd = 0;
	if (!cmd)
		return ;
	path = cmd->cmd_data->cmd_path;
	directory = opendir(path);
	fd = open(path, O_WRONLY);
	printf("fd: %d\n", fd);
	if (fd == -1 && directory)
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": is a directory", 2);
	}
	else if (path && (access(cmd->cmd_data->cmd_path, F_OK) == 0))
	{
		perror(cmd->cmd_data->cmd_path); //permission denied
		exit(126);
	}
	else if (cmd->cmd_data->cmd && path && (ft_strchr(cmd->cmd_data->cmd_path, '/')))
		perror(cmd->cmd_data->cmd[0]);
	else
	{
		ft_putstr_fd(cmd->cmd_data->cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	if (directory)
		closedir(directory);
	if (fd)
		close(fd);
	if (cmd->next == NULL)
		exit(127);
}