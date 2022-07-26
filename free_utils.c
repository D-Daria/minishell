/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:32:49 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/26 21:48:33 by mrhyhorn         ###   ########.fr       */
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
			(*cmd_head)->cmd_data->redir_in = NULL;
			(*cmd_head)->cmd_data->redir_out = NULL;
			(*cmd_head)->cmd_data->heredoc = NULL;
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

void	ft_free_data_ptr(t_data *data_ptr)
{
	ft_memdel(data_ptr->last_user_cmd);
	ft_free_list(&data_ptr->tokens);
	ft_free_redirs(&data_ptr->redirs);
	ft_free_commands(&data_ptr->commands);
	if (data_ptr->parser_ptr)
		ft_memdel(data_ptr->parser_ptr);
	if (data_ptr->path_by_launch)
		ft_free_split(data_ptr->path_by_launch);
}
