/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:43:03 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/25 23:04:46 by mrhyhorn         ###   ########.fr       */
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
	cmd_path = ft_access_paths(data->parser_ptr, (**token)->content->token);
	ft_get_cmd(&(token), &cmd);
	new_cmd = ft_new_cmd_lst(cmd_path, cmd, id, num);
	ft_lstadd_back(&data->commands, new_cmd);
	new_cmd = NULL;
	ft_free_split(cmd);
	ft_memdel(cmd_path);
}

static void	ft_get_args(t_data *data, t_list **token, int id, int num)
{
	t_list		*new_redir;

	new_redir = NULL;
	if (id == WORD) // определяем -> file, builtin, simple command
		ft_fill_command(data, &(token), id, num);
	else if (id < PIPE) // определяем редирект с файлом или heredoc с ограничителем
	{
		if ((*token)->next && (*token)->next->content->token_id == WORD)
		{
			new_redir = ft_new_redir_lst((*token)->next->content->token, id, num);
			ft_lstadd_back(&data->redirs, new_redir);
			new_redir = NULL;
			(*token) = (*token)->next;
		}
		else
		{
			new_redir = ft_new_redir_lst((*token)->content->token, id, num);
			ft_lstadd_back(&data->redirs, new_redir);
			new_redir = NULL;
		}
	}
}

void	ft_process_tokens(t_data *data)
{
	t_list		*current;
	int			id;
	int			num;

	num = 0;
	current = data->tokens;
	while (current)
	{
		id = current->content->token_id;
		if (id == WORD)
			ft_get_args(data, &current, WORD, ++num);
		if (!current)
			break;
		id = current->content->token_id;
		if (id >= L1_REDIRECT && id < PIPE)
			ft_get_args(data, &current, id, num);
		if (!current)
			break;
		else
			current = current->next;
	}
}

void ft_get_heredoc(t_data *data)
{
	t_list	*cmd;
	t_list	*redir;
	int		id;

	cmd = data->commands;
	id = 0;
	while (cmd)
	{
		redir = data->redirs;
		while (redir)
		{
			if (cmd->cmd_data->cmd_num == redir->redir_data->num)
			{
				if (redir->redir_data->id == L2_HEREDOC)
				{
					cmd->cmd_data->is_redir = 1;
					cmd->cmd_data->heredoc = redir;
					break ;
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

static void	ft_set_cmd_redirs(t_data *data)
{
	int		id;
	t_list	*cmd;
	t_list	*redir;

	cmd = data->commands;
	id = 0;
	while (cmd)
	{
		redir = data->redirs;
		while (redir)
		{
			if (cmd->cmd_data->cmd_num == redir->redir_data->num)
			{
				cmd->cmd_data->is_redir = 1;
				id = redir->redir_data->id;
				if (id == R1_REDIRECT || id == R2_REDIRECT)
					cmd->cmd_data->redir_out = redir;
				else if (id == L1_REDIRECT)
					cmd->cmd_data->redir_in = redir;
				else if (id == L2_HEREDOC)
					cmd->cmd_data->heredoc = redir;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	// ft_get_heredoc(data);
}

//начинаем разбивать токены на простые и встроенные команды (simple commands, builtins)
void	ft_commands(t_data *data)
{
	data->commands = NULL;
	data->redirs = NULL;
	// ft_print_list_of_tokens(data);
	ft_get_paths(data->parser_ptr);
	if (data->parser_ptr->paths == NULL)
		ft_error_exit("error getting paths\n");
	ft_process_tokens(data);
	ft_free_split(data->parser_ptr->paths);
	debug_print_redirections(data->redirs);
	if (data->commands && data->redirs)
		ft_set_cmd_redirs(data);
	printf("data->parser_ptr->paths cleared\n");
}
