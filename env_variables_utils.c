/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 15:37:16 by sshield           #+#    #+#             */
/*   Updated: 2022/08/01 22:43:17 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_getenv(t_data *data, char *var)
{
    t_list  *tmp;

    if (data->envplist != NULL)
    {
        tmp = data->envplist;
        while (tmp)
        {
            if (ft_strncmp(tmp->envp_str, var, ft_strlen(var)) == 0 \
                && (tmp->envp_str)[ft_strlen(var)] == '=')
                return (tmp->envp_str + ft_strlen(var) + 1);
            tmp = tmp->next;
        }
    }
    return (NULL);
}

void    ft_create_lstnew(t_list **l_new, char *str_new)
{
	*l_new = ft_calloc(1, sizeof(t_list));
    if (*l_new != NULL)
	    (*l_new)->envp_str = ft_strdup(str_new);
    if (*l_new == NULL || (*l_new)->envp_str == NULL)
        ft_error_exit("malloc_error in ft_create_lstnew\n");
}

void    ft_sorting_find_prev_list(t_list **prev, char *s_new, t_data *data)
{
	t_list	*current;
	char	*new_var;
	char	*old_var;
	size_t	len_new;
	size_t	len_old;

	ft_get_length_var(s_new, &len_new);
	new_var = ft_substr(s_new, 0, len_new);
	if (new_var == NULL)
		ft_error_exit("malloc_error\n");
	old_var = NULL;
	current = data->sorted_envplist;
	while (current)
	{
		ft_get_length_var(current->envp_str, &len_old);
		old_var = ft_substr(current->envp_str, 0, len_old);
		if (old_var == NULL)
			ft_error_exit("malloc_error\n");
		if (ft_strcmp(new_var, old_var) < 0)
			break ;
		*prev = current;
		current = current->next;
	}
	free (old_var);
	free (new_var);
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

void    ft_adding_var_to_envplist_if_flag(t_data *data, char *s_new)
{
    t_list  *new;

    // if (data->add_new_var_envplist == 0)
	// 	return ;
    ft_create_lstnew(&new, s_new);
	ft_lstadd_back(&data->envplist, new);
}
