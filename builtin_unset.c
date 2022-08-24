/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 14:12:05 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 19:33:18 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_delete_env_var(t_data *data, t_list **prev, t_list **var)
{
	if ((*prev) == NULL)
		data->envplist = (*var)->next;
	else
		(*prev)->next = (*var)->next;
	free ((*var)->envp_str);
	free (*var);
}

void	ft_unset_pwd_oldpwd(t_data *data, char *cmd)
{
	if (data->pwd_flag == 1 && (ft_strcmp(cmd, "PWD") == 0))
	{
		data->pwd_flag = 0;
		ft_memdel(data->pwd);
		data->pwd = ft_calloc(1, sizeof(char));
	}
	else if (data->oldpwd_flag == 1 && (ft_strcmp(cmd, "OLDPWD") == 0))
	{
		data->oldpwd_flag = 0;
		ft_memdel(data->oldpwd);
		data->oldpwd = ft_calloc(1, sizeof(char));
	}
}

void	ft_delete_if_found_in_envplist(char **tmp_cmd, t_data *data)
{
	t_list	*current_env;
	t_list	*prev_env;
	size_t	length;

	while (*tmp_cmd)
	{
		current_env = data->envplist;
		prev_env = NULL;
		while (current_env)
		{
			ft_get_length_var(current_env->envp_str, &length);
			if (ft_strncmp(current_env->envp_str, *tmp_cmd, length) == 0 \
				&& ((*tmp_cmd)[length] == '\0'))
			{
				ft_unset_pwd_oldpwd(data, *tmp_cmd);
				ft_delete_env_var(data, &prev_env, &current_env);
				data->amount_env_vars -= 1;
				ft_change_arr_env_vars(data);
				break ;
			}
			prev_env = current_env;
			current_env = current_env->next;
		}
		tmp_cmd += 1;
	}
}

void	ft_delete_if_found_in_sortlist(char **tmp_cmd, t_data *data)
{
	t_list	*current_env;
	t_list	*prev_env;
	size_t	length;

	while (*tmp_cmd)
	{
		current_env = data->sorted_envplist;
		prev_env = NULL;
		while (current_env)
		{
			ft_get_length_var(current_env->envp_str, &length);
			if (ft_strncmp(current_env->envp_str, *tmp_cmd, length) == 0 \
				&& ((*tmp_cmd)[length] == '\0'))
			{
				ft_unset_pwd_oldpwd(data, *tmp_cmd);
				ft_delete_env_var(data, &prev_env, &current_env);
				break ;
			}
			prev_env = current_env;
			current_env = current_env->next;
		}
		tmp_cmd += 1;
	}
}

void	ft_unset(t_data *data, t_list *cmd)
{
	char	**tmp_cmd;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return ;
	ft_delete_if_found_in_envplist(tmp_cmd, data);
	ft_delete_if_found_in_sortlist(tmp_cmd, data);
}
