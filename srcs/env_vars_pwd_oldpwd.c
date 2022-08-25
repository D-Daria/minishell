/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_pwd_oldpwd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:38:23 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 23:46:18 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_change_data_oldpwd(t_data *data, char *new_oldpwd)
{
	ft_memdel(data->oldpwd);
	data->oldpwd = ft_strdup(new_oldpwd);
	if (!data->oldpwd)
		ft_error_exit("malloc error\n");
}

void	ft_change_data_pwd(t_data *data, char *new_pwd)
{
	ft_memdel(data->pwd);
	data->pwd = ft_strdup(new_pwd);
	if (!data->pwd)
		ft_error_exit("malloc error\n");
}

void	ft_change_data_pwd_or_oldpwd(t_data *data, char *cmd)
{
	if (ft_strncmp(cmd, "PWD", 3) == 0 && cmd[3] == '=')
	{
		ft_change_data_pwd(data, cmd);
		data->pwd_flag = 1;
	}
	else if (ft_strncmp(cmd, "OLDPWD", 6) == 0 && cmd[6] == '=')
	{
		ft_change_data_oldpwd(data, cmd);
		data->oldpwd_flag = 1;
	}
}

void	ft_exp_pwd(t_data *data)
{
	char	*var;

	var = data->pwd;
	if (*data->pwd == '\0')
		var = "PWD";
	ft_change_envplist_if_var_found(var, 3, data);
	ft_change_sortlist_if_var_found(var, 3, data);
	if (data->add_new_var_envplist == 1 && (*data->pwd != '\0'))
	{
		ft_adding_var_to_envplist_if_flag(data, data->pwd);
		data->amount_env_vars += 1;
		ft_change_arr_env_vars(data);
	}
	if (*data->pwd == '\0')
		ft_adding_var_to_sortlist_if_flag(data, "PWD");
	else
		ft_adding_var_to_sortlist_if_flag(data, data->pwd);
	data->pwd_flag = 1;
}

void	ft_exp_oldpwd(t_data *data)
{
	char	*var;

	var = data->oldpwd;
	if (*data->oldpwd == '\0')
		var = "OLDPWD";
	ft_change_envplist_if_var_found(var, 6, data);
	ft_change_sortlist_if_var_found(var, 6, data);
	if (data->add_new_var_envplist == 1 && (*data->oldpwd != '\0'))
	{
		ft_adding_var_to_envplist_if_flag(data, data->oldpwd);
		data->amount_env_vars += 1;
		ft_change_arr_env_vars(data);
	}
	ft_adding_var_to_sortlist_if_flag(data, var);
	data->oldpwd_flag = 1;
}
