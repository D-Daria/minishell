/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 14:21:34 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/07 15:51:10 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* get_next_line
int	ft_process_heredoc(t_data *data, t_list *redir)
{
	int		pid;
	char	*line;
	char	*lim;
	int		fd;

	pid = fork();
	fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (pid == 0)
	{
		while (1)
		{
			ft_putstr_fd("> ", STDIN_FILENO);
			signal(SIGINT, &ft_signal_heredoc);
			line = get_next_line(0);
			lim = ft_strjoin(redir->redir_data->file, "\n");
			if (ft_strcmp(line, lim) == 0)
			{
				ft_memdel(lim);
				ft_memdel(line);
				exit(0);
			}
			ft_memdel(lim);
			if (line)
			{
				write(fd, line, ft_strlen(line));
				ft_memdel(line);
			}
			else
				exit(0);
		}
		close(fd);
	}
	else
		waitpid(pid, &data->status, 0);
	fd = open("here_doc", O_RDONLY);
	return (fd);
}
*/

/*
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
}*/

static void	ft_signal_heredoc(int signum)
{
	if (access("here_doc", F_OK) == 0)
		unlink("here_doc");
	exit(signum);
}

static void	ft_heredoc_child(t_sig sig, t_list *redir, int *fd)
{
	char	*line;

	sigaction(SIGINT, &sig, 0);
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
	t_sig	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_handler = ft_signal_heredoc;
	fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	pid = fork();
	if (pid == 0)
		ft_heredoc_child(sig, redir, &fd);
	close(fd);
	waitpid(pid, &data->status, 0);
	if (data->status > 0)
		signal(SIGINT, SIG_IGN);
	fd = open("here_doc", O_RDONLY);
	return (fd);
}
