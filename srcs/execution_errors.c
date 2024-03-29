/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 13:14:47 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 16:45:20 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_token_error(t_data *data, int id, int process)
{
	char	*token;

	(void)data;
	token = NULL;
	if (id == L1_REDIRECT)
		token = "`newline'";
	else if (id == L2_HEREDOC)
		token = "`newline'";
	else if (id == R1_REDIRECT)
		token = "`newline'";
	else if (id == R2_REDIRECT)
		token = "`newline'";
	else if (id == PIPE)
		token = "`|'";
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected token ", STDERR_FILENO);
	ft_putendl_fd(token, STDERR_FILENO);
	if (process)
		exit(258);
	else
		g_status = 258;
}

void	ft_file_error(t_data *data, char *file, int is_process)
{
	int	fd;

	(void)data;
	fd = open(file, O_RDWR);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
	}
	else
		close(fd);
	if (is_process)
		exit(1);
	else
		g_status = 1;
}

void	ft_perror_redir(t_data *data, t_list *redir)
{
	int	num;

	if (!redir || (g_status > 0))
		return ;
	num = redir->redir_data->num;
	if (num == -1)
		ft_token_error(data, redir->redir_data->id, 0);
	else if (redir->redir_data->file == NULL && num >= 0)
		ft_token_error(data, redir->redir_data->id, 0);
	else if (!data->commands && redir->redir_data->file)
		ft_file_error(data, redir->redir_data->file, 0);
	else if (num == 0 && redir->redir_data->file)
		ft_file_error(data, redir->redir_data->file, 0);
}

static void	ft_dir_error(t_list *cmd, char *path)
{
	int		fd;
	DIR		*directory;

	directory = opendir(path);
	fd = open(path, O_RDWR);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (access(cmd->cmd_data->cmd_path, F_OK) == 0)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (fd == -1 && directory)
	{
		closedir(directory);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		exit(1);
	}
	if (directory)
		closedir(directory);
	if (fd)
		close(fd);
	exit(126);
}

void	ft_perror(t_data *data, t_list *cmd)
{
	char	*path;

	(void)data;
	path = cmd->cmd_data->cmd_path;
	if (path)
		ft_dir_error(cmd, path);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->cmd_data->cmd[0], STDERR_FILENO);
	if ((access(cmd->cmd_data->cmd[0], F_OK) == 0))
	{
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
	if (!path && (ft_strchr(cmd->cmd_data->cmd[0], '/')))
	{
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		exit(126);
	}
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	if (cmd->next == NULL)
		exit(127);
}
