/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 22:05:12 by sshield           #+#    #+#             */
/*   Updated: 2022/08/13 12:57:27 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_spaces(t_data *data, char *cmd, char *rl_buf)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_memdel(data->last_user_cmd);
	data->last_user_cmd = NULL;
	ft_memdel(cmd);
	ft_memdel(rl_buf);
	return (1);
}

static void	ft_only_enter(t_data *data, char *rl_buffer)
{
	if (rl_buffer && !(*rl_buffer))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		ft_memdel(data->last_user_cmd);
		data->last_user_cmd = NULL;
	}
}

static bool	ft_not_empty(t_data *data, char **now_user_cmd, char *rl_buffer)
{
	*now_user_cmd = ft_strtrim(rl_buffer, " ");
	if (!(*now_user_cmd))
	{
		ft_memdel(rl_buffer);
		printf("malloc error in ft_read_userstring\n");
		ft_free_data_ptr(data);
		rl_clear_history();
		exit(-1);
	}
	if (*(*now_user_cmd) == '\0' && ft_spaces(data, *now_user_cmd, rl_buffer))
		return (1);
	if (ft_strcmp(*now_user_cmd, data->last_user_cmd) != 0)
		add_history(*now_user_cmd);
	ft_memdel(data->last_user_cmd);
	data->last_user_cmd = ft_strdup(*now_user_cmd);
	if (!data->last_user_cmd)
	{
		ft_memdel(*now_user_cmd);
		ft_memdel(rl_buffer);
		printf("malloc error in ft_read_userstring\n");
		ft_free_data_ptr(data);
		rl_clear_history();
		exit(-1);
	}
	return (0);
}

void	ft_read_user_cmd(t_data *data)
{
	char	*now_user_cmd;
	char	*rl_buffer;

	rl_buffer = readline(PROMPT);
	now_user_cmd = NULL;
	if (rl_buffer == NULL)
	{
		printf("exit\n");
		ft_free_data_ptr(data);
		rl_clear_history();
		exit(EXIT_SUCCESS);
	}
	if (rl_buffer && *rl_buffer && ft_not_empty(data, &now_user_cmd, rl_buffer))
		return ;
	ft_only_enter(data, rl_buffer);
	ft_memdel(now_user_cmd);
	ft_memdel(rl_buffer);
}
