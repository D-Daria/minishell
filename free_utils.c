/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:32:49 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/01 22:30:31 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_commands(t_list	**cmd_head)
{
	t_list	*next;

	if (*cmd_head == NULL)
		return ;
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
	// ft_memdel((*cmd_head));//не надо, или я ошибаюсь?
}

void	ft_free_redirs(t_list **redir_head)
{
	t_list	*next;

	if (*redir_head == NULL)
		return ;
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
	// ft_memdel((*redir_head));//не надо, или я ошибаюсь?
}

void	ft_free_tokenlist(t_list **list)
{
	t_list	*tmp;

	if (*list == NULL)
		return ;
	while (*list != NULL)
	{
		tmp = (*list)->next;
		free((*list)->content->token);
		free((*list)->content);
		(*list)->content = NULL;
		free ((*list));
		(*list) = NULL;
		(*list) = tmp;
	}
}

void	ft_free_envplist_or_free_sortlist(t_list **list)
{
	t_list	*tmp;

	if (*list == NULL)
		return ;
	while (*list != NULL)
	{
		tmp = (*list)->next;
		free((*list)->envp_str);
		(*list)->envp_str = NULL;
		free (*list);
		*list = NULL;
		*list = tmp;
	}
}

void	ft_free_data_ptr(t_data *data_ptr)
{
	ft_memdel(data_ptr->last_user_cmd);
	ft_free_tokenlist(&data_ptr->tokens);
	ft_free_redirs(&data_ptr->redirs);
	ft_free_commands(&data_ptr->commands);
	if (data_ptr->parser_ptr)
		ft_memdel(data_ptr->parser_ptr);
	ft_free_arr_env_vars(data_ptr->current_arr_env_vars);
	ft_free_envplist_or_free_sortlist(&data_ptr->envplist);
	ft_free_envplist_or_free_sortlist(&data_ptr->sorted_envplist);
}
