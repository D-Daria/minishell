#include <minishell.h>

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

void	ft_adding_var_to_sortlist_if_flag(t_data *data, char *s_new)
{
	t_list	*l_new;
	t_list	*tmp;
	t_list	*current;
	t_list	*prev;

    if (data->add_new_var_sortlist == 0)
		return ;
	current = data->sorted_envplist;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(s_new, current->envp_str) < 0)
			break ;
		prev = current;
		current = current->next;
	}
    ft_create_lstnew(&l_new, s_new);
	//-------------------------
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

    if (data->add_new_var_envplist == 0)
		return ;
    ft_create_lstnew(&new, s_new);
	ft_lstadd_back(&data->envplist, new);
}
