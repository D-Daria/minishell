/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 18:23:09 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 18:31:04 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_var_pwd(t_data *data)
{
	char	*pwd_path;
	char	*pwd;

	pwd_path = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", pwd_path);
	if (!pwd)
		ft_error_exit("malloc error\n");
	ft_memdel(pwd_path);
	data->add_new_var_sortlist = 1;
	ft_adding_var_to_sortlist_if_flag(data, pwd);
	ft_adding_var_to_envplist_if_flag(data, pwd);
	data->amount_env_vars += 1;
	data->pwd = ft_strdup(pwd);
	if (!data->pwd)
		ft_error_exit("malloc error\n");
	ft_memdel(pwd);
}

static bool	ft_pwd_or_oldpwd_is_found(t_data *data, char *envp)
{
	DIR	*dir;

	if (!data->oldpwd_flag && ft_strncmp(envp, "OLDPWD=", 7) == 0)
	{
		data->oldpwd_flag = 1;
		dir = opendir(envp + 7);
		if (dir == NULL)
			return (1);
		closedir(dir);
		data->oldpwd = ft_strdup(envp);
		if (!data->oldpwd)
			ft_error_exit("malloc error\n");
	}
	else if (!data->pwd_flag && ft_strncmp(envp, "PWD=", 4) == 0)
	{
		data->pwd_flag = 1;
		return (1);
	}
	return (0);
}

static void	ft_oldpwd_is_not_found(t_data *data)
{
	data->add_new_var_sortlist = 1;
	ft_adding_var_to_sortlist_if_flag(data, "OLDPWD");
	data->oldpwd = ft_strdup("OLDPWD");
	if (!data->oldpwd)
		ft_error_exit("malloc error\n");
}

static void	ft_create_envplists(t_data *data, char **envp)
{
	int	i;

	if (!envp)
		ft_error_exit("envp==NULL in ft_create_envplist\n");
	ft_init_var_pwd(data);
	i = 0;
	while (envp[i])
	{
		if (ft_pwd_or_oldpwd_is_found(data, envp[i]) == 1)
		{
			i++;
			continue ;
		}
		ft_adding_var_to_envplist_if_flag(data, envp[i]);
		data->amount_env_vars += 1;
		data->add_new_var_sortlist = 1;
		ft_adding_var_to_sortlist_if_flag(data, envp[i]);
		i++;
	}
	ft_change_arr_env_vars(data);
	if (data->oldpwd_flag == 0)
		ft_oldpwd_is_not_found(data);
}

void	ft_init(t_data *data_ptr, char **envp)
{
	data_ptr->parser_ptr = ft_calloc(1, sizeof(t_parser));
	if (!data_ptr->parser_ptr)
	{
		printf("Malloc error. Minishell has been stopped\n");
		exit(-1);
	}
	ft_create_envplists(data_ptr, envp);
	ft_set_builtins(data_ptr);
	data_ptr->oldpwd_flag = 1;
	data_ptr->pwd_flag = 1;
}
