/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_listshandling.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 13:21:11 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 02:57:22 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_change_envplist_if_var_found(char *var, size_t l_name, t_data *data)
{
	t_list	*env;
	char	*new_var;
	char	*old_var;

	ft_init_change_func(data, &env, &old_var, 1);
	new_var = ft_substr(var, 0, l_name);
	if (ft_check_in_change_envplist(data, new_var, var[l_name]))
		return ;
	while (env)
	{
		old_var = ft_get_name_env_var(env->envp_str);
		if (ft_strcmp(new_var, old_var) == 0)
		{
			if (ft_strcmp(env->envp_str + l_name, var + l_name) == 0 && \
				ft_not_change(data, 1, old_var, new_var))
				return ;
			ft_change_var(&env, var, data, 'e');
			ft_change_arr_env_vars(data);
			ft_memdel(old_var);
			break ;
		}
		ft_memdel(old_var);
		env = env->next;
	}
	ft_memdel(new_var);
}

void	ft_change_sortlist_if_var_found(char *var, size_t l, t_data *data)
{
	t_list	*lst;
	char	*new_var;
	char	*old_var;

	ft_init_change_func(data, &lst, &old_var, 0);
	new_var = ft_substr(var, 0, l);
	if (!new_var)
		ft_error_exit("malloc_error\n");
	while (lst)
	{
		old_var = ft_get_name_env_var(lst->envp_str);
		if (ft_strcmp(old_var, new_var) == 0)
		{
			if (ft_strcmp(lst->envp_str + l, var + l) == 0 && \
				ft_not_change(data, 0, old_var, new_var))
				return ;
			ft_change_var(&lst, var, data, 's');
			ft_memdel(old_var);
			break ;
		}
		lst = lst->next;
		ft_memdel(old_var);
	}
	ft_memdel(new_var);
}

void	ft_sorting_find_prev_list(t_list **prev, char *s_new, t_data *data)
{
	t_list	*current;
	char	*new_var;
	char	*old_var;

	new_var = ft_get_name_env_var(s_new);
	old_var = NULL;
	current = data->sorted_envplist;
	while (current)
	{
		old_var = ft_get_name_env_var(current->envp_str);
		if (ft_strcmp(new_var, old_var) < 0)
		{
			ft_memdel(old_var);
			break ;
		}
		*prev = current;
		current = current->next;
		ft_memdel(old_var);
	}
	ft_memdel(new_var);
}

void	ft_adding_var_to_sortlist_if_flag(t_data *data, char *s_new)
{
	t_list	*l_new;
	t_list	*tmp;
	t_list	*prev;

	if (data->add_new_var_sortlist == 0)
		return ;
	prev = NULL;
	ft_sorting_find_prev_list(&prev, s_new, data);
	ft_create_lstnew(&l_new, s_new);
	if (!prev)
	{
		ft_lstadd_front(&(data->sorted_envplist), l_new);
		return ;
	}
	tmp = prev->next;
	prev->next = l_new;
	l_new->next = tmp;
}

void	ft_adding_var_to_envplist_if_flag(t_data *data, char *s_new)
{
	t_list	*new;

	ft_create_lstnew(&new, s_new);
	ft_lstadd_back(&data->envplist, new);
}
