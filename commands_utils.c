/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 18:56:44 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/23 17:47:55 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_commands(t_list	**cmd_head)
{
	t_list	*next;

	if (*cmd_head == NULL)
	{
		printf("commands are empty\n");
		return ;
	}
	if (*cmd_head != NULL)
	{
		while (*cmd_head != NULL)
		{
			next = (*cmd_head)->next;
			ft_free_split((*cmd_head)->cmd_data->cmd);
			ft_memdel((*cmd_head)->cmd_data->cmd_path);
			ft_memdel((*cmd_head)->cmd_data);
			ft_memdel((*cmd_head));
			(*cmd_head) = next;
		}
	}
	ft_memdel((*cmd_head));
	printf("commands cleared\n");
}

void	ft_free_redirs(t_list **redir_head)
{
	t_list	*next;

	if (*redir_head == NULL)
	{
		printf("redirs are empty\n");
		return ;
	}
	if (*redir_head != NULL)
	{
		while (*redir_head != NULL)
		{
			next = (*redir_head)->next;
			ft_memdel((*redir_head)->redir_data->file);
			ft_memdel((*redir_head)->redir_data);
			ft_memdel((*redir_head));
			(*redir_head) = next;
		}
	}
	ft_memdel((*redir_head));
	printf("redirs cleared\n");
}

t_command	*ft_create_command(char *cmd_path, char **cmd_args, int id, int num)
{
	t_command	*new_cmd;
	size_t		i;

	new_cmd = (t_command *)ft_calloc(sizeof(t_command), 1);
	if (new_cmd == NULL)
		return (NULL);
	if (cmd_args)
	{
		i = ft_split_len(cmd_args);
		new_cmd->cmd = (char **)malloc(sizeof(char *) * (i + 1));
		if (new_cmd->cmd == NULL)
			return (NULL);
		i = 0;
		while (cmd_args[i])
		{
			new_cmd->cmd[i] = ft_strdup(cmd_args[i]);
			i++;
		}
		new_cmd->cmd[i] = NULL;
	}
	else
		new_cmd->cmd = NULL;
	// i = ft_split_len(cmd_args);
	// new_cmd->cmd = (char **)malloc(sizeof(char *) * (i + 1));
	// if (new_cmd->cmd == NULL)
	// 	return (NULL);
	// i = 0;
	// while (cmd_args[i])
	// {
	// 	new_cmd->cmd[i] = ft_strdup(cmd_args[i]);
	// 	i++;
	// }
	// new_cmd->cmd[i] = NULL;
	new_cmd->num = num;
	new_cmd->cmd_path = ft_strdup(cmd_path);
	new_cmd->token_id = id;
	return (new_cmd);
}

t_list	*ft_new_cmd_lst(char *cmd_path, char **cmd_args, int id, int num)
{
	t_list		*cmd_lst;
	t_command	*new_cmd;

	// if (id < WORD)
	// {
	// 	new_cmd = (t_command *)malloc(sizeof(t_command) * 1);
	// 	new_cmd->token_id = id;
	// 	new_cmd->cmd_path = NULL;
	// 	new_cmd->cmd = NULL;
	// }
	// else
		new_cmd = ft_create_command(cmd_path, cmd_args, id, num);
	if (new_cmd == NULL)
		return (NULL);
	cmd_lst = (t_list *)malloc(sizeof(t_list) * 1);
	if (cmd_lst == NULL)
		return (NULL);
	cmd_lst->cmd_data = new_cmd;
	cmd_lst->next = NULL;
	return (cmd_lst);
}

char	*ft_access_paths(t_parser *parser, char *cmd)
{
	int		i;
	char	*join;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (parser->paths[i])
	{
		join = ft_strjoin(parser->paths[i], cmd);
		if (access(join, X_OK) == 0)
			return (join);
		else
			free(join);
		i++;
	}
	return (NULL);
}

void	ft_get_paths(t_parser *parser)
{
	char	*env_path;
	char	**path;
	char	*join;
	int		i;

	env_path = getenv("PATH");
	path = ft_split(env_path, ':');
	if (path == NULL)
		return ;
	i = 0;
	while (path[i])
		i++;
	parser->paths = (char **)malloc(sizeof(char *) * (i + 1));
	if (parser->paths == NULL)
		return ;
	i = 0;
	while (path[i])
	{
		join = ft_strjoin(path[i], "/");
		parser->paths[i++] = ft_strdup(join);
		free(join);
	}
	parser->paths[i] = NULL;
	ft_free_split(path);
}
