/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 23:41:18 by sshield           #+#    #+#             */
/*   Updated: 2022/08/13 16:54:52 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			printf("ft_double_quotes: Wrong usage. \
			We don't handle unclosed quote\n");
			exit (EXIT_SUCCESS);
		}
		else if (str[*i] == '$')
		{
			ft_add_this_str_to_word(data, i, start_token);
			ft_add_expand_dollar_to_word(data, str, i);
			*start_token = *i;
			continue ;
		}
		*i = (*i) + 1;
	}
	ft_add_this_str_to_word(data, i, start_token);
	*i = (*i) + 1;
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
			printf("ft_single_quotes: Wrong usage. \
			We don't handle unclosed quote\n");
			exit (EXIT_SUCCESS);
		}
		*i = (*i) + 1;
	}
	ft_add_this_str_to_word(data, i, start_token);
	*i = (*i) + 1;
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
