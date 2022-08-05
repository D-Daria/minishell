/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 21:50:23 by sshield           #+#    #+#             */
/*   Updated: 2022/08/02 16:21:53 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_expand_dollar_question(t_data *data)
{
	data->expand_dollar = ft_itoa(data->status);
	if (!data->expand_dollar)
		ft_error_exit("malloc_error in ft_itoa\n");
}

void	ft_expand_dollar(t_data *data, char *str, size_t *i)
{
	unsigned int	start;
	char 			*str_after_dollar;

	// if (data->expand_dollar)
	// 	ft_memdel(data->expand_dollar);
	if (str[(*i) + 1] == '?')
	{	
		ft_expand_dollar_question(data);
		*i += 2;
		return ;
	}
	start = *i + 1;
	*i = *i + 1;
	if (str[*i] != '_' && ft_isalnum((int)str[*i]) == 0)
	{//expand = $
		data->expand_dollar = (char *)malloc(sizeof(char) * 2);
		(data->expand_dollar)[0] = '$';
		(data->expand_dollar)[1] = '\0';
		return ;
	}
	while (ft_isalnum((int)str[*i]) == 1 || str[*i] == '_')
		*i = (*i) + 1;
	str_after_dollar = ft_substr(str, start, *i - start);
	if (!str_after_dollar)
		ft_error_exit("ft_expand_dollar: malloc error\n");
	char *tmp = ft_getenv(data, str_after_dollar);
	if (tmp == NULL)
		data->expand_dollar = NULL;
	else 
		data->expand_dollar = ft_strdup(tmp);//check malloc
	free (str_after_dollar);
}

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

void	ft_add_expand_dollar_to_word(t_data *data, char *str, size_t *i)
{
	char	*tmp;

	ft_expand_dollar(data, str, i);//malloc в data->expand_dollar
	if (data->expand_dollar == NULL)
		return ;
	tmp = data->last_token->content->token;
	data->last_token->content->token = ft_strjoin(tmp, data->expand_dollar);
	if (data->last_token->content->token == NULL)
		ft_error_exit("malloc_error in ft_add_dollar_to_word\n");
	ft_memdel(tmp);
	ft_memdel(data->expand_dollar);
}

void	ft_dubble_quotes(t_data *data, size_t *i, size_t *start_token)
{
	char	*str;

	str = data->last_user_cmd;
	*start_token = (*i) + 1;
	*i = (*i) + 1;
	while (str[*i] != '"')
	{
		if (str[*i] == '\0')
		{
			printf("ft_double_quotes: Wrong usage. We don't handle unclosed quote\n");
			exit (EXIT_SUCCESS);
		}
		else if (str[*i] == '$')
		{
			ft_add_this_str_to_word(data, i, start_token);
			ft_add_expand_dollar_to_word(data, str, i);//сместит i последоллар
			*start_token = *i;
			continue ;
		}
		*i = (*i) + 1;
	}
	ft_add_this_str_to_word(data, i, start_token);
	*i =(*i) + 1;
	*start_token = *i;
}

void	ft_single_quotes(t_data *data, size_t *i, size_t *start_token)
{
	char	*str;

	*start_token = (*i) + 1;
	*i = (*i) + 1;
	str = data->last_user_cmd;
	while (str[*i] != '\'')
	{
		if (str[*i] == '\0')
		{
			printf("ft_single_quotes: Wrong usage. We don't handle unclosed quote\n");
			exit (EXIT_SUCCESS);
		}
		*i = (*i) + 1;
	}
	ft_add_this_str_to_word(data, i, start_token);
	*i =(*i) + 1;
	*start_token = *i;
}

char	ft_is_next_symbol_the_same(char *str, size_t *i, size_t *start)
{
	if ((str[*i] == '\'' && str[(*i) + 1] == '\'') || \
		(str[*i] == '"' && str[(*i) + 1] == '"'))
	{
		*i = *i + 2;
		*start = *i;
		return ('y');
	}
	return ('n');
}

void	ft_parse_quotes(t_data *data, size_t *i, size_t *st)
{
	char	*str;

	str = data->last_user_cmd;
	ft_add_this_str_to_word(data, i, st);
	if (ft_is_next_symbol_the_same(str, i, st) == 'y')
		return ;
	if (str[*i] == '\'')
		ft_single_quotes(data, i, st);
	else if (str[*i] == '"')
		ft_dubble_quotes(data, i, st);
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
	//создаю новый токен с пустой строкой и помечаю id=WORD
	ft_create_token(data, WORD, i, start_token);
	data->last_token->content->token = (char *)malloc(sizeof(char));
	if (!data->last_token->content->token)
		ft_error_exit("malloc error in ft_find_first_word\n");
	(data->last_token->content->token)[0] = '\0';
}

void	ft_parse_by_delimitter(t_data *data, size_t *i, size_t *st)
{
	char	*str;

	str = data->last_user_cmd;
	if (data->last_token->content->token_id == WORD)
		ft_add_this_str_to_word(data, i, st);
	if (data->last_token->content->token_id == WORD && *data->last_token->content->token == '\0')
	{
		//надо удалить последний элемент из списка. Список односвязынй.
		t_list *ptr; ptr = data->tokens; int last_indx = -1;
		while (ptr)
		{
			last_indx++;
			ptr = ptr->next; 
		}
		ptr = data->tokens; 
		if (last_indx == 0)
		{
			data->tokens = NULL; free (ptr->content); free (ptr); return ;}
		//иду до предпоследнего и перенаправляю в null предпоследний.

		while(last_indx-- > 1)
			ptr = ptr->next;
		ptr->next = NULL;
		free (data->last_token->content);
		data->last_token = NULL;
	}
	if (str[*i] == ' ')
		*i += 1;
	else if (str[*i] == '\0')
		return ;
	else 
		ft_create_token_pipe_redir_heredoc(data, i, st);
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
	while(str[i])
	{
		if (str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == ' ')
		{
			ft_parse_by_delimitter(data, &i, &start_token);
			if(str[i] == '\0')
				return ;
			ft_find_token_word(data, &i, &start_token);
		}
		else if (str[i] == '$')
		{
			ft_add_this_str_to_word(data, &i, &start_token);
			ft_add_expand_dollar_to_word(data, str, &i);//смещает i
			start_token = i;
		}
		else if (str[i] == '\'' || str[i] == '"')
			ft_parse_quotes(data, &i, &start_token);
		else 
			i++;
	}
	ft_parse_by_delimitter(data, &i, &start_token);
}
