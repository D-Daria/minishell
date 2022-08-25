/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 23:49:51 by sshield           #+#    #+#             */
/*   Updated: 2022/08/13 14:24:40 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_expand_dollar_question(t_data *data)
{
	data->expand_dollar = ft_itoa(g_status);
	if (!data->expand_dollar)
		ft_error_exit("malloc_error in ft_itoa\n");
}

char	*ft_expand_after_dollar(t_data *d, char *cmd, size_t *i, size_t *start)
{
	char	*str_after_dollar;

	if (cmd[(*i) + 1] == '?')
	{	
		ft_expand_dollar_question(d);
		*i += 2;
		return (NULL);
	}
	*start = *i + 1;
	*i += 1;
	if (cmd[*i] != '_' && ft_isalnum((int)cmd[*i]) == 0)
	{
		d->expand_dollar = (char *)malloc(sizeof(char) * 2);
		if (!d->expand_dollar)
			ft_error_exit("malloc error\n");
		(d->expand_dollar)[0] = '$';
		(d->expand_dollar)[1] = '\0';
		return (NULL);
	}
	while (ft_isalnum((int)cmd[*i]) == 1 || cmd[*i] == '_')
		*i = (*i) + 1;
	str_after_dollar = ft_substr(cmd, *start, *i - *start);
	if (!str_after_dollar)
		ft_error_exit("ft_expand_dollar: malloc error\n");
	return (str_after_dollar);
}

static bool	ft_is_pwd_or_oldpwd(t_data *data, char *str)
{
	char	*help;

	if (ft_strncmp(str, data->pwd, 3) == 0 && str[3] == '\0')
	{
		help = ft_substr(data->pwd, 4, ft_strlen(data->pwd) - 4);
		data->expand_dollar = ft_strdup(help);
		ft_memdel(help);
		ft_memdel(str);
		return (1);
	}
	else if (ft_strncmp(str, data->oldpwd, 6) == 0 && str[6] == '\0')
	{
		help = ft_substr(data->oldpwd, 7, ft_strlen(data->oldpwd) - 7);
		data->expand_dollar = ft_strdup(help);
		if (!data->exp_dol_flag)
			ft_error_exit("malloc error\n");
		ft_memdel(help);
		ft_memdel(str);
		return (1);
	}
	return (0);
}

void	ft_expand_dollar(t_data *data, char *str, size_t *i)
{
	size_t	start;
	char	*expanded;
	char	*tmp;

	expanded = ft_expand_after_dollar(data, str, i, &start);
	if (!expanded)
		return ;
	if (ft_is_pwd_or_oldpwd(data, expanded))
		return ;
	tmp = ft_getenv(data, expanded);
	if (tmp == NULL)
		data->expand_dollar = NULL;
	else
	{
		data->expand_dollar = ft_strdup(tmp);
		if (!data->expand_dollar)
			ft_error_exit("malloc error\n");
	}
	ft_memdel(expanded);
}

void	ft_add_expand_dollar_to_word(t_data *data, char *str, size_t *i)
{
	char	*tmp;

	data->exp_dol_flag = 1;
	ft_expand_dollar(data, str, i);
	if (data->expand_dollar == NULL)
		return ;
	tmp = data->last_token->content->token;
	data->last_token->content->token = ft_strjoin(tmp, data->expand_dollar);
	if (data->last_token->content->token == NULL)
		ft_error_exit("malloc_error in ft_add_dollar_to_word\n");
	ft_memdel(tmp);
	ft_memdel(data->expand_dollar);
}
