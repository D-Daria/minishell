/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:43:03 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/22 18:42:33 by mrhyhorn         ###   ########.fr       */
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

/*
static void	ft_get_args(t_data *data, t_list **token, int id)
{
	t_parser	*parser;
	char		**cmd;
	char		*cmd_path;
	t_list		*new_cmd;

	parser = data->parser_ptr;
	cmd = NULL;
	cmd_path = ft_access_paths(parser, (*token)->content->token);
	if (!cmd_path)
	{
		printf("token: %s\n", (*token)->content->token);
		ft_check_file((*token)->content->token, &id, &cmd_path);
	}
	if ((*token)->next == NULL || id < WORD)
	{
		cmd = (char **)malloc(sizeof(char *) * 2);
		if (cmd == NULL)
			return ;
		cmd[0] = ft_strdup((*token)->content->token);
		cmd[1] = NULL;
	}
	else
		ft_get_cmd(token, &cmd);
	new_cmd = ft_new_cmd_lst(cmd_path, cmd, id);
	ft_lstadd_back(&data->commands, new_cmd);
	new_cmd = NULL; printf("cmd cleared\n");
	ft_free_split(cmd);
	ft_memdel(cmd_path);
}
*/

/*	TODO
	учесть сигналы в дочерних процессах
	
*/

static void	ft_get_args(t_data *data, t_list **token, int id)
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
		// cmd = (char **)malloc(sizeof(char *) * 2);
		// if (cmd == NULL)
		// 	return ;
		if ((*token)->next)
		{
			(*token) = (*token)->next;
			cmd_path = ft_strdup((*token)->content->token);
			// cmd[0] = ft_strdup((*token)->content->token);
		}
		// else
		// 	cmd[0] = ft_strdup((*token)->content->token);
		// cmd[1] = NULL;
	}
	new_cmd = ft_new_cmd_lst(cmd_path, cmd, id);
	ft_lstadd_back(&data->commands, new_cmd);
	new_cmd = NULL; printf("cmd cleared\n");
	ft_free_split(cmd);
	ft_memdel(cmd_path);
}

void	ft_process_tokens(t_data *data)
{
	t_list		*current;
	int			id;

	current = data->tokens;
	while (current)
	{
		id = current->content->token_id;
		if (id == WORD)
			ft_get_args(data, &current, WORD);
		if (!current)
			break;
		id = current->content->token_id;
		if (id >= L1_REDIRECT && id < PIPE)
			ft_get_args(data, &current, id);
		printf("current->token: %s\n", current->content->token);
		if (!current)
			break;
		else
			current = current->next;
	}
}

//начинаем разбивать токены на простые и встроенные команды (simple commands, builtins)
void	ft_commands(t_data *data)
{
	data->commands = NULL;
	// ft_print_list_of_tokens(data);
	ft_get_paths(data->parser_ptr);
	if (data->parser_ptr->paths == NULL)
		ft_error_exit("error getting paths\n");
	ft_process_tokens(data);
	ft_free_split(data->parser_ptr->paths);
	printf("data->parser_ptr->paths cleared\n");
}
