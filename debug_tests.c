/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:48 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/25 12:35:40 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_double_arr(char **arr)
{
	int i;

	if (arr)
	{
		i = 0;
		while(arr[i])
		{
			printf("[%d]: %s\n", i, arr[i]);
			i++;
		}
	}
	else
		printf("%s\n", NULL);
}

void	debug_print_commands_list(t_data *data)
{
	t_list	*cmd_lst;
	int		i;

	if (!data->commands)
		printf("Commands are empty\n");
	cmd_lst = data->commands;
	i = 0;
	while (cmd_lst)
	{
		printf("%slist #%d%s\n",YELLOW, i + 1, BREAK);
		printf("cmd: ");
		debug_print_double_arr(cmd_lst->cmd_data->cmd);
		printf("cmd_path: %s\n", cmd_lst->cmd_data->cmd_path);
		printf("num: %d\n", cmd_lst->cmd_data->cmd_num);
		printf("cmd_id: %d\n", cmd_lst->cmd_data->cmd_id);
		if (cmd_lst->cmd_data->cmd_redir_in)
		{
			printf("redir_in: %s\n", "TRUE");
			debug_print_redirections(cmd_lst->cmd_data->cmd_redir_in);
		}
		else
			printf("redir_in: %s\n", "FALSE");
		if (cmd_lst->cmd_data->cmd_redir_out)
		{
			printf("redir_out: %s\n", "TRUE");
			debug_print_redirections(cmd_lst->cmd_data->cmd_redir_out);
		}
		else
			printf("redir_out: %s\n", "FALSE");
		i++;
		cmd_lst = cmd_lst->next;
		printf("\n");
	}
}

void	debug_print_redirections(t_list *redirs)
{
	t_list	*redir;

	redir = redirs;
	if (!redirs)
		printf("No redirections\n");
	while (redirs)
	{
		printf(GREEN"redirs\n"BREAK);
		printf("num: %d\n", redirs->redir_data->num);
		printf("file: %s\n", redirs->redir_data->file);
		printf("fd: %d\n", redirs->redir_data->fd);
		printf("id: %d\n", redirs->redir_data->id);
		redirs = redirs->next;
	}
}

void	ft_print_list_of_tokens(t_data *data)
{	
	t_list	*ptr;

	printf("Печатаю лист:\n");
	if (!data->tokens)
	{
		printf("	list пустой!\n");
		return ;
	}
	ptr = data->tokens;
	while (ptr)
	{
		printf("[	");
		printf("token={%s}   id=%d", ptr->content->token, ptr->content->token_id);
		printf("	]\n");
		ptr = ptr->next;
	}
	printf("Лист распечатан\n\n");
}
