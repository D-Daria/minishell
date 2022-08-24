/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 21:50:23 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 21:42:10 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_find_token_word(t_data *data, size_t *i, size_t *start_token)
{
	char	*str;

	str = data->last_user_cmd;
	while (str[*i] == '<' || str[*i] == '>' || str[*i] == '|' || str[*i] == ' ')
	{
		if (str[*i] == ' ')
			*i += 1;
		else
			ft_create_token_pipe_redir_heredoc(data, i, start_token);
	}
	if (!str[*i])
		return ;
	ft_create_token(data, WORD, i, start_token);
	data->last_token->content->token = (char *)malloc(sizeof(char));
	if (!data->last_token->content->token)
		ft_error_exit("malloc error in ft_find_first_word\n");
	(data->last_token->content->token)[0] = '\0';
	data->exp_dol_flag = 0;
}

static void	ft_del_last_token(t_data *data)
{
	t_list	*ptr;
	int		last_indx;

	ptr = data->tokens;
	last_indx = -1;
	while (ptr)
	{
		last_indx++;
		ptr = ptr->next;
	}
	ptr = data->tokens;
	if (last_indx == 0)
	{
		data->tokens = NULL;
		ft_memdel(ptr->content->token);
		ft_memdel(ptr->content);
		ft_memdel(ptr);
		return ;
	}
	while (last_indx-- > 1)
		ptr = ptr->next;
	ptr->next = NULL;
	ft_memdel(data->last_token->content->token);
	ft_memdel(data->last_token->content);
	ft_memdel(data->last_token);
}

void	ft_parse_by_delimitter(t_data *data, size_t *i, size_t *st)
{
	char	*str;

	str = data->last_user_cmd;
	if (data->last_token->content->token_id == WORD)
		ft_add_this_str_to_word(data, i, st);
	if (data->last_token->content->token_id == WORD && \
		*data->last_token->content->token == '\0' && \
		data->exp_dol_flag == 1)
		ft_del_last_token(data);
	if (str[*i] == ' ')
		*i += 1;
	else if (str[*i] == '\0')
		return ;
	else
		ft_create_token_pipe_redir_heredoc(data, i, st);
}

bool	ft_char_parsing(t_data *data, char *str, size_t *i, size_t *start)
{
	if (str[*i] == '<' || str[*i] == '>' || str[*i] == '|' || str[*i] == ' ')
	{
		ft_parse_by_delimitter(data, i, start);
		if (str[*i] == '\0')
			return (1);
		ft_find_token_word(data, i, start);
	}
	else if (str[*i] == '$')
	{
		ft_add_this_str_to_word(data, i, start);
		ft_add_expand_dollar_to_word(data, str, i);
		*start = *i;
	}
	else if (str[*i] == '\'' || str[*i] == '"')
		ft_parse_quotes(data, i, start);
	else
		*i += 1;
	return (0);
}

void	ft_parser(t_data *data)
{
	char	*str;
	size_t	i;
	size_t	start_token;

	str = data->last_user_cmd;
	data->pipes_number = 0;
	start_token = 0;
	i = 0;
	ft_find_token_word(data, &i, &start_token);
	while (str[i])
	{
		if (ft_char_parsing(data, str, &i, &start_token))
			return ;
	}
	ft_parse_by_delimitter(data, &i, &start_token);
}
