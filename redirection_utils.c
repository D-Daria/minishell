/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:17:47 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/29 14:17:21 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_new_redir_lst(char *file, int id, int num)
{
	t_list	*redir_lst;
	t_redir *redir;

	redir = (t_redir *)ft_calloc(sizeof(t_redir), 1);
	if (!redir)
		return (NULL);
	redir->num = num;
	redir->file = ft_strdup(file);
	redir->id = id;
	redir_lst = (t_list *)ft_calloc(sizeof(t_list), 1);
	if (!redir_lst)
		return (NULL);
	redir_lst->redir_data = redir;
	redir_lst->next = NULL;
	return (redir_lst);
}

int	ft_process_heredoc(t_list *redir)
{
	char	*line;
	char	*lim;
	int		fd;

	lim = redir->redir_data->file;
	fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	line = readline("> ");
	while (line)
	{
		if (ft_strcmp(line, lim) == 0)
			break ;
		line = ft_strjoin(line, "\n");
		write(fd, line, ft_strlen(line));
		ft_memdel(line);
		line = readline("> ");
	}
	close(fd);
	fd = open("here_doc", O_RDONLY);
	return (fd);
}

int	ft_open_files(t_data *data, t_list *redir, int id, int is_process)
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
	else if (id == L2_HEREDOC)
		fd = ft_process_heredoc(redir);
	if (fd == -1 && is_process)
		ft_file_error(data, file_name, is_process);
	return (fd);
}


void	ft_process_redirs(t_data *data)
{
	t_list	*redir;
	int		id;
	int		num;

	printf("process redirs\n");
	redir = data->redirs;
	while (redir)
	{
		id = redir->redir_data->id;
		num = redir->redir_data->num;
		if (num == -1)
			ft_perror_redir(data, redir);
		if (redir->redir_data->file == NULL && num >= 0)
			ft_perror_redir(data, redir);
		redir->redir_data->fd = ft_open_files(data, redir, id, 0);
		if (redir->redir_data->fd < 0)
			ft_perror_redir(data, redir);
		redir = redir->next;
	}
}
