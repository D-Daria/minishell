/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:08:37 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/23 01:00:11 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redirect(t_data *data, int *fd_read, t_list *cmd, int id)
{
	(void)fd_read;
	(void)cmd;
	if (id == R1_REDIRECT)
	{
		// printf("data->fd: %d\n", data->fd);
		dup2(data->fd_in, STDOUT_FILENO);
	}
}

int	ft_check_files(t_data *data, t_list *cmd, int *fd_read, int id)
{
	char	*file_name;

	if (cmd->cmd_data->cmd_path == NULL)
		return (-1);
	file_name = cmd->cmd_data->cmd_path;
	printf("file_name: %s\n", file_name);
	if (id == L1_REDIRECT)
		data->fd_out = open(file_name, O_RDONLY);
	else if (id == R1_REDIRECT)
		data->fd_in = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else if (id == R2_REDIRECT)
		data->fd_in = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
	// printf("data->fd: %d\n", data->fd);
	if (data->fd_in == -1 || data->fd_out == -1)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(" : ", STDERR_FILENO);
		ft_putstr_fd(file_name, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		ft_close_all(data, *fd_read, NULL);
		exit(1);
	}
	return (1);
}