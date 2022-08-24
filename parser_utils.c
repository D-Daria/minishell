/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 22:55:43 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 21:40:43 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_this_str_to_word(t_data *data, size_t *i, size_t *st)
{
	char	*tmp;
	char	*tmp2;
	char	*str;

	str = data->last_user_cmd;
	tmp = data->last_token->content->token;
	tmp2 = ft_substr(str, *st, *i - *st);
	if (tmp2 == NULL)
		ft_error_exit("malloc_error in ft_add_dollar_to_word\n");
	data->last_token->content->token = ft_strjoin(tmp, tmp2);
	ft_memdel(tmp);
	ft_memdel(tmp2);
	if (data->last_token->content->token == NULL)
		ft_error_exit("malloc_error in ft_add_dollar_to_word\n");
}

t_list	*ft_calloc_new_token(void)
{
	t_list				*list;
	t_content_for_list	*lstcontent;

	lstcontent = (t_content_for_list *)ft_calloc(1, sizeof(t_content_for_list));
	if (!lstcontent)
		ft_error_exit("malloc error in ft_malloc_content_list\n");
	list = ft_lstnew(lstcontent);
	if (!list)
		ft_error_exit("malloc error in ft_malloc_content_list\n");
	return (list);
}

void	ft_create_token(t_data *data, int id, size_t *i, size_t *st)
{
	t_list	*new;

	new = ft_calloc_new_token();
	data->last_token = new;
	new->content->token_id = id;
	ft_lstadd_back(&data->tokens, new);
	if (id == WORD)
	{
		*st = *i;
		return ;
	}
	if (id == L2_HEREDOC || id == R2_REDIRECT)
		*i += 1;
	*i += 1;
}

void	ft_create_token_pipe_redir_heredoc(t_data *data, size_t *i, size_t *st)
{
	char	*str;

	str = data->last_user_cmd;
	if (str[*i] == '|')
	{
		ft_create_token(data, PIPE, i, st);
		data->pipes_number += 1;
	}
	else if (str[*i] == '>' && str[(*i) + 1] != '>')
		ft_create_token(data, R1_REDIRECT, i, st);
	else if (str[*i] == '<' && str[(*i) + 1] != '<')
		ft_create_token(data, L1_REDIRECT, i, st);
	else if (str[*i] == '<' && str[(*i) + 1] == '<')
		ft_create_token(data, L2_HEREDOC, i, st);
	else if (str[*i] == '>' && str[(*i) + 1] == '>')
		ft_create_token(data, R2_REDIRECT, i, st);
}
