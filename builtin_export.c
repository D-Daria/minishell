/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 19:42:51 by sshield           #+#    #+#             */
/*   Updated: 2022/08/01 23:18:31 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_arr_env_vars(char ***arr)
{
	// size_t	i;

	// if (!(*arr))
	// 	return ;
	// i = 0;
	// while ((*arr)[i])
	// 	ft_memdel((*arr)[i++]);
	(void)arr;
}

void	ft_change_arr_env_vars(t_data *data)
{
	t_list	*tmp;
	char	**old;
	size_t	i;

	old = data->current_arr_env_vars;
	ft_free_arr_env_vars(&old);
	data->current_arr_env_vars = (char **)malloc(sizeof(char *) \
	* data->amount_env_vars + 1);
	if (!data->current_arr_env_vars)
		ft_error_exit("malloc_error in ft_create_envplists\n");
	tmp = data->envplist;
	i = 0;
	while (tmp)
	{
		(data->current_arr_env_vars)[i] = tmp->envp_str;
		tmp = tmp->next;
		i++;
	}
	(data->current_arr_env_vars)[i] = NULL;
//check:
	// i=0;
	// while((data->current_arr_env_vars)[i])
	// 	printf("%s\n",(data->current_arr_env_vars)[i++]);
}

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

void	ft_change_var(t_list **lst, char *new, t_data *data, char name_list)
{
	char	*old;

	if (name_list == 's')
		data->add_new_var_sortlist = 0;
	else if (name_list == 'e')
		data->add_new_var_envplist = 0;
	old = (*lst)->envp_str;
	(*lst)->envp_str = ft_strdup(new);
	if ((*lst)->envp_str == NULL)
		ft_error_exit("malloc_error in ft_change_var\n");
	free (old);
}

void	ft_change_envplist_if_var_found(char *var, size_t l_var, t_data *data)
{
    t_list	*env;
    char    *shift;
	char	*new_var;
	char	*old_var;
	size_t	len_old;

    env = data->envplist;
	data->add_new_var_envplist = 1;
	new_var = ft_substr(var, 0, l_var);
	old_var = NULL;
    while (env)
	{
		ft_get_length_var(env->envp_str, &len_old);
		old_var = ft_substr(env->envp_str, 0, len_old);
		if (ft_strcmp(new_var, old_var) == 0)
		{
            shift = env->envp_str + len_old;
            if (ft_strcmp(shift, var + len_old) == 0)
            {
                data->add_new_var_envplist = 0;
				free (new_var);
				free (old_var);
                return ;
            }
	    	ft_change_var(&env, var, data, 'e');
			break ;
		}
		env = env->next;
	}
	free (new_var);
	free (old_var);
}

void	ft_change_sortlist_if_var_found(char *var, size_t l, t_data *data)
{
    t_list	*lst;
    char    *shift;
	char	*new_var;
	char	*old_var;
	size_t	len_old;

    lst = data->sorted_envplist;
	data->add_new_var_sortlist = 1;
	new_var = ft_substr(var, 0, l);
	old_var = NULL;
    while (lst)
	{
		ft_get_length_var(lst->envp_str, &len_old);
		old_var = ft_substr(lst->envp_str, 0, len_old);
		if (ft_strcmp(old_var, new_var) == 0)
		{
			if (var[l] == '\0')
			{
				data->add_new_var_sortlist = 0;
				free (old_var);
				free (new_var);
				return ;
			}
            shift = lst->envp_str + l;
            if (ft_strcmp(shift, var + l) == 0)
            {
                data->add_new_var_sortlist = 0;
				free (old_var);
				free (new_var);
                return ;
            }
	    	ft_change_var(&lst, var, data, 's');
			break ;
		}
		lst = lst->next;
	}
	free (new_var);
	free (old_var);
}

int    ft_check_varerrors(char *var, t_data *data, size_t *length)
{
    size_t  i;

	(void)data;
    if (!ft_isalpha((int)*var) && *var != '_')
    {
        printf("export: переменная «%s» начинается не с буквы и не с '_'\n", var);
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

void	ft_export(t_data *data, t_list *cmd)
{
	char	**tmp_cmd;
	size_t	length;
	int		ret;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return (ft_export_without_args(data));
	while (*tmp_cmd)
	{
		ret = ft_check_varerrors(*tmp_cmd, data, &length);
		if (ret == 1)
		{
			ft_change_envplist_if_var_found(*tmp_cmd, length, data);
			ft_change_arr_env_vars(data);
			ft_change_sortlist_if_var_found(*tmp_cmd, length, data);
			if (data->add_new_var_envplist == 1)
			{
				ft_adding_var_to_envplist_if_flag(data, *tmp_cmd);
				ft_change_arr_env_vars(data);
			}
			ft_adding_var_to_sortlist_if_flag(data, *tmp_cmd);
		}
		else if (ret == 0)
		{
			ft_change_sortlist_if_var_found(*tmp_cmd, length, data);
			ft_adding_var_to_sortlist_if_flag(data, *tmp_cmd);
		}
		tmp_cmd += 1;
	}
}
