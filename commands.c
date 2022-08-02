/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 14:13:57 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/02 21:38:41 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_fill_command(t_data *data, t_list ***token, int id, int num)
{
	char		**cmd;
	char		*cmd_path;
	t_list		*new_cmd;

	cmd = NULL;
	cmd_path = NULL;
	new_cmd = NULL;
	ft_get_cmd(&(token), &cmd);
	cmd_path = ft_access_paths(data->parser_ptr, *cmd);
	new_cmd = ft_new_cmd_lst(cmd_path, cmd, id, num);
	ft_lstadd_back(&data->commands, new_cmd);
	new_cmd = NULL;
	ft_free_split(cmd);
	ft_memdel(cmd_path);
	data->cmds_number += 1;
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
	if (id == WORD) // определяем -> file, builtin, simple command
		ft_fill_command(data, &(token), id, num);
	else if (id >= L1_REDIRECT && id < PIPE) // определяем редирект с файлом или heredoc с ограничителем
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
			break;
		id = current->content->token_id;
		if (id >= L1_REDIRECT && id < PIPE)
			ft_get_args(data, &current, prev, num);
		if (id == PIPE)
			ft_get_args(data, &current, prev, ++num);
		if (!current)
			break;
		prev = current;
		current = current->next;
	}
}

static void	ft_set_cmd_redirs(t_data *data)
{
	t_list	*cmd;
	t_list	*redir;
	int		id;

	cmd = data->commands;
	while (cmd)
	{
		redir = data->redirs;
		while (redir)
		{
			if (cmd->cmd_data->cmd_num == redir->redir_data->num)
			{
				cmd->cmd_data->is_redir = 1;
				id = redir->redir_data->id;
				if (id == L1_REDIRECT)
					cmd->cmd_data->redir_in = redir;
				if (id == R1_REDIRECT || id == R2_REDIRECT)
					cmd->cmd_data->redir_out = redir;
				if (id == L2_HEREDOC)
					cmd->cmd_data->heredoc = redir;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

//начинаем разбивать токены на простые и встроенные команды (simple commands, builtins)
void	ft_commands(t_data *data)
{
	t_list	*token;
	t_list	*prev;

	data->commands = NULL;
	data->redirs = NULL;
	token = NULL;
	prev = NULL;
	// ft_print_list_of_tokens(data);
	ft_get_paths(data, data->parser_ptr);
	ft_process_tokens(data, token, prev);
	ft_free_split(data->parser_ptr->paths);
	// debug_print_redirections(data->redirs);
	if (data->commands && data->redirs)
		ft_set_cmd_redirs(data);
	printf("data->parser_ptr->paths cleared\n");
}
