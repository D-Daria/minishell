/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 19:42:51 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 19:08:48 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export_without_args(t_data *data)
{
	t_list	*current;
	size_t	i;

	current = data->sorted_envplist;
	while (current)
	{
		printf("declare -x ");
		i = 0;
		while ((current->envp_str)[i] && (current->envp_str)[i] != '=')
			printf("%c", (current->envp_str)[i++]);
		if (!(current->envp_str)[i])
		{
			printf("\n");
			current = current->next;
			continue ;
		}
		printf("=\"%s\"\n", current->envp_str + i + 1);
		current = current->next;
	}
}

int	ft_check_varerrors(char *var, t_data *data, size_t *length)
{
	size_t	i;

	(void)data;
	if (!ft_isalpha((int)*var) && *var != '_')
	{
		printf("export %s: Varname does not begin with 'Aa-Zz' or '_'\n", var);
		return (2);
	}
	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
		{
			*length = i;
			return (1);
		}
		else if (!ft_isalnum((int)var[i]) && var[i] != '_')
		{
			printf("export «%s» wrong format varname, 'Aa-Zz 0-9 _'\n", var);
			return (2);
		}
		i++;
	}
	*length = i;
	return (0);
}

bool	ft_export_pwd_oldpwd(t_data *data, char ***cmd)
{
	if (ft_strcmp(**cmd, "PWD") == 0)
		ft_exp_pwd(data);
	else if (ft_strcmp(**cmd, "OLDPWD") == 0)
		ft_exp_oldpwd(data);
	else
		return (0);
	*cmd += 1;
	return (1);
}

void	ft_processing_exported_vars(t_data *data, char ***tmp_cmd)
{
	int		ret;
	size_t	length;

	ret = ft_check_varerrors(**tmp_cmd, data, &length);
	if (ret == 1)
	{
		ft_change_envplist_if_var_found(**tmp_cmd, length, data);
		ft_change_sortlist_if_var_found(**tmp_cmd, length, data);
		if (data->add_new_var_envplist == 1)
		{
			ft_adding_var_to_envplist_if_flag(data, **tmp_cmd);
			data->amount_env_vars += 1;
			ft_change_arr_env_vars(data);
		}
		ft_adding_var_to_sortlist_if_flag(data, **tmp_cmd);
		ft_change_data_pwd_or_oldpwd(data, **tmp_cmd);
	}
	else if (ret == 0)
	{
		if (ft_export_pwd_oldpwd(data, tmp_cmd) == 1)
			return ;
		ft_change_sortlist_if_var_found(**tmp_cmd, length, data);
		ft_adding_var_to_sortlist_if_flag(data, **tmp_cmd);
	}
	*tmp_cmd += 1;
}

void	ft_export(t_data *data, t_list *cmd)
{
	char	**tmp_cmd;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return (ft_export_without_args(data));
	while (*tmp_cmd)
		ft_processing_exported_vars(data, &tmp_cmd);
}
