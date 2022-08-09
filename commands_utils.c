/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 18:56:44 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/09 13:35:05 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_get_cmd(t_list ****token, char ***cmd)
{
	size_t	n;
	t_list	*tmp;

	tmp = ***token;
	n = 0;
	while (tmp && (tmp)->content->token_id == WORD)
	{
		(tmp) = (tmp)->next;
		n++;
	}
	*cmd = (char **)malloc(sizeof(char *) * (n + 1));
	n = 0;
	while ((***token) && (***token)->content->token_id == WORD)
	{
		(*cmd)[n] = ft_strdup((***token)->content->token);
		// printf("33: %s\n", (***token)->content->token);
		(***token) = (***token)->next;
		n++;
	}
	(*cmd)[n] = NULL;
}

void	ft_fill_command(t_data *data, t_list ***token, int id, int num)
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

t_command	*ft_create_command(char **args)
{
	t_command	*new_cmd;
	size_t		i;

	new_cmd = (t_command *)ft_calloc(sizeof(t_command), 1);
	if (new_cmd == NULL)
		return (NULL);
	if (args)
	{
		i = ft_split_len(args);
		new_cmd->cmd = (char **)malloc(sizeof(char *) * (i + 1));
		if (new_cmd->cmd == NULL)
			return (NULL);
		i = 0;
		while (args[i])
		{
			new_cmd->cmd[i] = ft_strdup(args[i]);
			i++;
		}
		new_cmd->cmd[i] = NULL;
	}
	else
		new_cmd->cmd = NULL;
	return (new_cmd);
}

t_list	*ft_new_cmd_lst(char *cmd_path, char **cmd_args, int id, int num)
{
	t_list		*cmd_lst;
	t_command	*new_cmd;

	new_cmd = ft_create_command(cmd_args);
	if (new_cmd == NULL)
		return (NULL);
	new_cmd->cmd_num = num;
	new_cmd->cmd_path = ft_strdup(cmd_path);
	new_cmd->cmd_id = id;
	cmd_lst = (t_list *)malloc(sizeof(t_list) * 1);
	if (cmd_lst == NULL)
		return (NULL);
	cmd_lst->cmd_data = new_cmd;
	cmd_lst->next = NULL;
	return (cmd_lst);
}
