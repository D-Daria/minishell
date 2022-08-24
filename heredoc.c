/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 14:21:34 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:19:30 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_signal_heredoc(int signum)
{
	if (access("here_doc", F_OK) == 0)
		unlink("here_doc");
	exit(signum);
}

static void	ft_heredoc_child(t_list *redir, int *fd)
{
	char	*line;

	ft_signal(SIGINT, &ft_signal_heredoc);
	line = readline("> ");
	while (line)
	{
		if (ft_strcmp(line, redir->redir_data->file) == 0)
			break ;
		line = ft_strjoin(line, "\n");
		write(*fd, line, ft_strlen(line));
		ft_memdel(line);
		line = readline("> ");
	}
	exit(0);
}

int	ft_process_heredoc(t_data *data, t_list *redir)
{
	int		fd;
	int		pid;

	(void)data;
	fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	pid = fork();
	if (pid == 0)
		ft_heredoc_child(redir, &fd);
	close(fd);
	waitpid(pid, &g_status, 0);
	fd = open("here_doc", O_RDONLY);
	return (fd);
}
