/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 13:52:57 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/09 15:16:29 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_change_envp(t_data *data, char *var, size_t len)
{
	ft_change_envplist_if_var_found(var, len, data);
	ft_change_sortlist_if_var_found(var, len, data);
	ft_change_arr_env_vars(data);
}

static void	ft_update_path(t_data *data)
{
	char	*path;
	char	*new_pwd;
	char	*old_pwd;

	old_pwd = ft_getenv(data, "PWD");
	if (old_pwd)
	{
		new_pwd = ft_strjoin("OLDPWD=", old_pwd);//check malloc
		printf("old_pwd: %s\n", new_pwd);
		ft_change_envp(data, new_pwd, 6);
		ft_memdel(new_pwd);
	}
	path = getcwd(NULL, 0);
	new_pwd = ft_strjoin("PWD=", path);//check malloc
	ft_change_envp(data, new_pwd, 3);
	ft_memdel(new_pwd);
	ft_memdel(path);
}

static void	ft_home(t_data *data, char *home)
{
	int		ret;

	ret = chdir(home);
	if (ret == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		data->status = 1;
	}
	else if (ret == 0)
		ft_update_path(data);
}

void	ft_cd_process(t_data *data, t_list *cmd)
{
	char	*arg;
	char	*home;
	int		ret;

	home = ft_getenv(data, "HOME");
	arg = cmd->cmd_data->cmd[1];
	if (arg)
	{
		if (ft_strcmp(arg, "~") == 0)
			arg = home;
		ret = chdir(arg);
		if (ret == -1 && arg)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(arg);
			data->status = 1;
		}
		else if (ret == 0)
			ft_update_path(data);
	}
	else if (!arg)
		ft_home(data, home);
}
