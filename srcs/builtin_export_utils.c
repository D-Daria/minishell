/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 12:09:19 by sshield           #+#    #+#             */
/*   Updated: 2022/08/12 01:19:27 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_create_lstnew(t_list **l_new, char *str_new)
{
	*l_new = ft_calloc(1, sizeof(t_list));
	if (*l_new != NULL)
		(*l_new)->envp_str = ft_strdup(str_new);
	if (*l_new == NULL || (*l_new)->envp_str == NULL)
		ft_error_exit("malloc_error in ft_create_lstnew\n");
}

bool	ft_not_change(t_data *d, bool env, char *old, char *new)
{
	if (env == 1)
		d->add_new_var_envplist = 0;
	else
		d->add_new_var_sortlist = 0;
	ft_memdel(old);
	ft_memdel(new);
	return (1);
}

void	ft_change_var(t_list **lst, char *new, t_data *data, char name_list)
{
	char	*tmp;

	if (name_list == 's')
		data->add_new_var_sortlist = 0;
	else if (name_list == 'e')
		data->add_new_var_envplist = 0;
	tmp = (*lst)->envp_str;
	(*lst)->envp_str = ft_strdup(new);
	if ((*lst)->envp_str == NULL)
		ft_error_exit("malloc_error in ft_change_var\n");
	ft_memdel(tmp);
}

void	ft_init_change_func(t_data *d, t_list **l, char **old, bool env)
{
	*old = NULL;
	if (env == 1)
	{
		*l = d->envplist;
		d->add_new_var_envplist = 1;
	}
	else
	{
		*l = d->sorted_envplist;
		d->add_new_var_sortlist = 1;
	}
}

bool	ft_new_var_is_empty(char last_ch, t_data *d, char *old, char *new)
{
	if (last_ch == '\0')
	{
		d->add_new_var_sortlist = 0;
		ft_memdel(old);
		ft_memdel(new);
		return (1);
	}
	return (0);
}
