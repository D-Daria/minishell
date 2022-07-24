/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:43:03 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/24 20:59:19 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_get_cmd(t_list **token, char ***cmd)
{
	char		*join;
	char		*space;
	char		*str_spaced;

	str_spaced = NULL;
	while ((*token) && (*token)->content->token_id == WORD)
	{
		if ((*token)->content->token == NULL)
			break ;
		space = ft_strjoin(" ", str_spaced);
		ft_memdel(str_spaced);
		join = ft_strjoin((*token)->content->token, space);
		ft_memdel(space);
		str_spaced = ft_strdup(join);
		ft_memdel(join);
		(*token) = (*token)->next;
	}
	*cmd = ft_strrev_split(str_spaced, ' ');
	ft_memdel(str_spaced);
}

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

/*
static void	ft_get_args(t_data *data, t_list **token, int id, int num)
{
	char		**cmd;
	char		*cmd_path;
	t_list		*new_cmd;

	cmd = NULL;
	cmd_path = NULL;
	if (id == WORD) // определяем -> file, builtin, simple command
	{
		cmd_path = ft_access_paths(data->parser_ptr, (*token)->content->token);
		ft_get_cmd(token, &cmd);
	}
	if (id < PIPE) // определяем редирект с файлом или heredoc с ограничителем
	{
		if ((*token)->next)
		{
			(*token) = (*token)->next;
			cmd_path = ft_strdup((*token)->content->token);
		}
	}
	new_cmd = ft_new_cmd_lst(cmd_path, cmd, id, num);
	ft_lstadd_back(&data->commands, new_cmd);
	new_cmd = NULL; printf("cmd cleared\n");
	ft_free_split(cmd);
	ft_memdel(cmd_path);
}
*/

static void	ft_get_args(t_data *data, t_list **token, int id, int num)
{
	char		**cmd;
	char		*cmd_path;
	t_list		*new_cmd;
	t_list		*new_redir;

	cmd = NULL;
	cmd_path = NULL;
	new_redir = NULL;
	new_cmd = NULL;
	if (id == WORD) // определяем -> file, builtin, simple command
	{
		cmd_path = ft_access_paths(data->parser_ptr, (*token)->content->token);
		ft_get_cmd(token, &cmd);
		new_cmd = ft_new_cmd_lst(cmd_path, cmd, id, num);
		ft_lstadd_back(&data->commands, new_cmd);
		new_cmd = NULL;
	}
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
	ft_free_split(cmd);
	ft_memdel(cmd_path);
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

void	ft_set_redirs(t_data *data)
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
				id = redir->redir_data->id;
				if (id == R1_REDIRECT || id == R2_REDIRECT)
					cmd->cmd_data->cmd_redir_out = redir;
				else if (id == L1_REDIRECT || id == L2_HEREDOC)
					cmd->cmd_data->cmd_redir_in = redir;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
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
		ft_set_redirs(data);
	printf("data->parser_ptr->paths cleared\n");
}
