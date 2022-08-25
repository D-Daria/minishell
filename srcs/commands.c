/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 14:13:57 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:09:11 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_set_cmd_redirs(t_data *data)
{
	t_list	*cmd;
	int		cmd_num;

	cmd = data->commands;
	while (cmd)
	{
		cmd_num = cmd->cmd_data->cmd_num;
		ft_set_redir(data, &cmd->cmd_data->redir_in, cmd_num, L1_REDIRECT);
		ft_set_redir(data, &cmd->cmd_data->redir_out, cmd_num, R1_REDIRECT);
		ft_set_redir(data, &cmd->cmd_data->redir_out, cmd_num, R2_REDIRECT);
		ft_set_redir(data, &cmd->cmd_data->heredoc, cmd_num, L2_HEREDOC);
		if (cmd->cmd_data->redir_in || cmd->cmd_data->redir_out \
			|| cmd->cmd_data->heredoc)
		{
			cmd->cmd_data->is_redir = 1;
		}
		cmd = cmd->next;
	}
}

void	ft_fill_redir(t_data *data, t_list ***token, int id, int num)
{
	t_list		*new_redir;

	new_redir = NULL;
	if ((**token)->next && (**token)->next->content->token_id == WORD)
	{
		(**token) = (**token)->next;
		new_redir = ft_new_redir_lst((**token)->content->token, id, num);
		ft_lstadd_back(&data->redirs, new_redir);
		new_redir = NULL;
	}
	else
	{
		new_redir = ft_new_redir_lst((**token)->content->token, id, num);
		ft_lstadd_back(&data->redirs, new_redir);
		new_redir = NULL;
	}
}

static void	ft_get_args(t_data *data, t_list **token, t_list *prev, int num)
{
	t_list	*new_redir;
	int		id;

	new_redir = NULL;
	id = (*token)->content->token_id;
	if (id == WORD)
		ft_fill_command(data, &(token), id, num);
	else if (id >= L1_REDIRECT && id < PIPE)
		ft_fill_redir(data, &token, id, num);
	else if (id == PIPE)
	{
		if (prev == NULL)
			new_redir = ft_new_redir_lst(NULL, id, -1);
		else
			new_redir = ft_new_redir_lst(NULL, id, -2);
		ft_lstadd_back(&data->redirs, new_redir);
		new_redir = NULL;
	}
}

void	ft_process_tokens(t_data *data, t_list *current, t_list *prev)
{
	int		id;
	int		num;

	num = 1;
	current = data->tokens;
	while (current)
	{
		if (current->content->token_id == WORD)
		{
			prev = current;
			ft_get_args(data, &current, prev, num);
		}
		if (!current)
			break ;
		id = current->content->token_id;
		if (id >= L1_REDIRECT && id < PIPE)
			ft_get_args(data, &current, prev, num);
		if (id == PIPE)
			ft_get_args(data, &current, prev, ++num);
		if (!current)
			break ;
		prev = current;
		current = current->next;
	}
}

void	ft_commands(t_data *data)
{
	t_list	*token;
	t_list	*prev;

	data->commands = NULL;
	data->redirs = NULL;
	token = NULL;
	prev = NULL;
	ft_get_paths(data, data->parser_ptr);
	ft_process_tokens(data, token, prev);
	ft_free_split(data->parser_ptr->paths);
}
