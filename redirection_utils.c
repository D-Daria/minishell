/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:17:47 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/25 14:18:03 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_open_files(t_data *data, t_list *redir, int id, int process)
{
	int		fd;
	char	*file_name;

	if (redir->redir_data->file == NULL)
		return (-1);
	file_name = redir->redir_data->file;
	fd = -2;
	if (id == L1_REDIRECT)
		fd = open(file_name, O_RDONLY);
	else if (id == R1_REDIRECT)
		fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else if (id == R2_REDIRECT)
		fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd == -1 && process)
		ft_file_error(data, file_name, process);
	return (fd);
}
