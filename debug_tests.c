/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:48 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/22 15:18:13 by mrhyhorn         ###   ########.fr       */
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
		printf(YELLOW);
		printf("list #%d\n", i + 1);
		printf(BREAK);
		printf("cmd: ");
		debug_print_double_arr(cmd_lst->cmd_data->cmd);
		printf("cmd_path: ");
		printf("%s\n", cmd_lst->cmd_data->cmd_path);
		printf("cmd_id: ");
		printf("%d\n", cmd_lst->cmd_data->token_id);
		i++;
		cmd_lst = cmd_lst->next;
		printf("\n");
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
