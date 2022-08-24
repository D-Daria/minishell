/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_open.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:47:38 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/12 19:32:48 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_num(t_data *data, int redir_num)
{
	t_list	*cmds;

	cmds = data->commands;
	while (cmds)
	{
		if (cmds->cmd_data->cmd_num == redir_num)
			return (1);
		cmds = cmds->next;
	}
	return (0);
}

static void	ft_redir_num(t_data *data, t_list **redir, int num)
{
	int	id;
	int	error;

	error = 0;
	while ((*redir) && (*redir)->redir_data->num == num)
	{
		id = (*redir)->redir_data->id;
		if (id == R1_REDIRECT || id == R2_REDIRECT || id == L1_REDIRECT)
		{
			if ((*redir)->redir_data->file == NULL && num >= 0)
				ft_perror_redir(data, (*redir));
			if (!error)
				(*redir)->redir_data->fd = ft_open_files(data, (*redir), id, 0);
			if ((*redir)->redir_data->fd < 0)
			{
				error = 1;
				if (!ft_is_num(data, (*redir)->redir_data->num))
					ft_file_error(data, (*redir)->redir_data->file, 0);
			}
		}
		(*redir) = (*redir)->next;
	}
}

void	ft_open_redirs(t_data *data)
{
	t_list	*redir;
	int		num;
	int		id;

	redir = data->redirs;
	while (redir)
	{
		num = redir->redir_data->num;
		if (num == -1)
			ft_perror_redir(data, redir);
		if (redir->redir_data->file == NULL && num >= 0)
			ft_perror_redir(data, redir);
		if (g_status == 258)
			return ;
		id = redir->redir_data->id;
		if (id == L1_REDIRECT || id == R1_REDIRECT || id == R2_REDIRECT)
			ft_redir_num(data, &redir, num);
		if (!redir)
			break ;
		redir = redir->next;
	}
}

void	ft_open_heredoc(t_data *data)
{
	t_list	*redir;
	int		id;
	int		num;

	redir = data->redirs;
	while (redir)
	{
		num = redir->redir_data->num;
		id = redir->redir_data->id;
		if (num == -1)
			ft_perror_redir(data, redir);
		if (redir->redir_data->file == NULL && num >= 0)
			ft_perror_redir(data, redir);
		if (id == L2_HEREDOC)
		{
			redir->redir_data->fd = ft_open_files(data, redir, id, 0);
			if (redir->redir_data->fd < 0)
				ft_perror_redir(data, redir);
		}
		redir = redir->next;
	}
}

void	ft_set_redir(t_data *data, t_list **cmd_redir, int cmd_num, int type)
{
	t_list	*redir;
	int		num;
	int		id;

	redir = data->redirs;
	id = 0;
	while (redir)
	{
		id = redir->redir_data->id;
		num = redir->redir_data->num;
		if (num == cmd_num && id == type)
		{
			(*cmd_redir) = redir;
			if (id == R1_REDIRECT || id == R2_REDIRECT)
				break ;
		}
		redir = redir->next;
	}
}
