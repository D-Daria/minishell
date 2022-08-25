/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 13:52:57 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:40:34 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_change_envp(t_data *data, char *var, size_t len)
{
	ft_change_envplist_if_var_found(var, len, data);
	ft_change_sortlist_if_var_found(var, len, data);
	if (data->add_new_var_envplist == 1)
	{
		ft_adding_var_to_envplist_if_flag(data, var);
		data->amount_env_vars += 1;
		ft_change_arr_env_vars(data);
	}
	ft_adding_var_to_sortlist_if_flag(data, var);
}

static void	ft_update_path(t_data *data, char *old_pwd)
{
	char	*new_pwd;
	char	*pwd_path;

	if (data->oldpwd_flag)
	{
		ft_change_envp(data, old_pwd, 6);
		data->oldpwd_flag = 1;
	}
	ft_change_data_oldpwd(data, old_pwd);
	pwd_path = getcwd(NULL, 0);
	new_pwd = ft_strjoin("PWD=", pwd_path);
	if (!new_pwd)
		ft_error_exit("malloc error\n");
	ft_memdel(pwd_path);
	if (data->pwd_flag)
	{
		ft_change_envp(data, new_pwd, 3);
		data->pwd_flag = 1;
	}
	ft_change_data_pwd(data, new_pwd);
	ft_memdel(new_pwd);
}

static void	ft_home(t_data *data, char *home, char *old_pwd)
{
	int		ret;

	if (home == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		g_status = 1;
	}
	else
	{
		ret = chdir(home);
		if (ret == -1)
		{
			ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
			perror(home);
			g_status = 1;
		}
		else if (ret == 0)
			ft_update_path(data, old_pwd);
	}
}

static char	*ft_get_old_pwd(t_data *data)
{
	char	*tmp;
	char	*old_pwd;

	tmp = ft_substr(data->pwd, 4, ft_strlen(data->pwd) - 4);
	if (!tmp)
		ft_error_exit("mallloc error\n");
	if (*tmp == '\0')
		old_pwd = ft_strdup("OLDPWD");
	else
		old_pwd = ft_strjoin("OLDPWD=", tmp);
	if (!old_pwd)
		ft_error_exit("malloc error\n");
	ft_memdel(tmp);
	return (old_pwd);
}

void	ft_cd_process(t_data *data, t_list *cmd)
{
	char	*arg;
	int		ret;
	char	*old_pwd;

	old_pwd = ft_get_old_pwd(data);
	arg = cmd->cmd_data->cmd[1];
	if (arg && ft_strcmp(arg, "~") != 0)
	{
		ret = chdir(arg);
		if (ret == -1 && arg)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(arg);
			g_status = 1;
		}
		else if (ret == 0)
			ft_update_path(data, old_pwd);
	}
	else
		ft_home(data, ft_getenv(data, "HOME"), old_pwd);
	ft_memdel(old_pwd);
}
