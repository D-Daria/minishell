/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 15:37:16 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 14:26:37 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_arr_env_vars(char **arr)
{
	size_t	i;

	if (!(arr))
		return ;
	i = 0;
	while ((arr)[i])
		ft_memdel((arr)[i++]);
	ft_memdel(arr);
}

void	ft_change_arr_env_vars(t_data *data)
{
	t_list	*tmp;
	size_t	i;

	ft_free_arr_env_vars(data->current_arr_env_vars);
	data->current_arr_env_vars = (char **)malloc(sizeof(char *) \
	* (data->amount_env_vars + 1));
	if (!data->current_arr_env_vars)
		ft_error_exit("malloc_error in ft_create_envplists\n");
	tmp = data->envplist;
	i = 0;
	while (tmp)
	{
		(data->current_arr_env_vars)[i] = ft_strdup(tmp->envp_str);
		if (!(data->current_arr_env_vars)[i])
			ft_error_exit("malloc_error\n");
		tmp = tmp->next;
		i++;
	}
	(data->current_arr_env_vars)[i] = NULL;
}

char	*ft_getenv(t_data *data, char *var)
{
	t_list	*tmp;
	size_t	len;

	len = ft_strlen(var);
	if (data->envplist != NULL)
	{
		tmp = data->envplist;
		while (tmp)
		{
			if (ft_strncmp(tmp->envp_str, var, len) == 0 \
				&& (tmp->envp_str)[len] == '=')
				return (tmp->envp_str + len + 1);
			tmp = tmp->next;
		}
	}
	return (NULL);
}

void	ft_get_length_var(char *cmd, size_t *length)
{
	*length = 0;
	while (cmd[*length] != '=' && cmd[*length] != '\0')
		*length += 1;
}

char	*ft_get_name_env_var(char *cmd)
{
	size_t	length;
	char	*name_var;

	length = 0;
	while (cmd[length] != '=' && cmd[length])
		length += 1;
	name_var = ft_substr(cmd, 0, length);
	if (!name_var)
		ft_error_exit("malloc_error\n");
	return (name_var);
}
