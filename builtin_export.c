#include "minishell.h"

void	ft_export_without_args(t_data *data)
{
	t_list	*current;
	size_t	i;

	printf("NO args\n\n");
	current = data->sorted_envplist;
	while (current)
	{
		printf("declare x ");
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

void	ft_change_var(t_list **lst, char *new, t_data *data)
{
	char	*old;

	data->ready_create_new_var = 0;
	old = (*lst)->envp_str;
	(*lst)->envp_str = ft_strdup(new);//malloc
	free (old);
	//-------------------

	printf("CHANGE\n");
}

void	ft_change_envplist_if_var_found(char *var, size_t l, t_data *data)
{
    t_list	*env;
    char    *shift;

    env = data->envplist;
    while (env)
	{
		if (ft_strncmp(env->envp_str, var, l) == 0)
		{
            shift = env->envp_str + l;
            if (ft_strcmp(shift, var + l) == 0)
            {
                data->ready_create_new_var = 0;
                return ;
            }
	    	ft_change_var(&env, var, data);
			break ;
		}
		env = env->next;
	}
}

void	ft_change_sortlist_if_var_found(char *var, size_t l, t_data *data)
{
    t_list	*lst;
    char    *shift;

    lst = data->sorted_envplist;
    while (lst)
	{
		if (ft_strncmp(lst->envp_str, var, l) == 0)
		{
            shift = lst->envp_str + l;
            if (ft_strcmp(shift, var + l) == 0)
            {
                data->ready_create_new_var = 0;
                return ;
            }
	    	ft_change_var(&lst, var, data);
			break ;
		}
		lst = lst->next;
	}
}

int    ft_check_varerrors_init_flag(char *var, t_data *data, size_t *length)
{
    size_t  i;

    data->ready_create_new_var = 0;
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
			data->ready_create_new_var = 1;
            *length = i + 1;
            printf("ok\n");
            return (1);
        }
        else if (!ft_isalnum((int)var[i]) && var[i] != '_') 
        {
            printf("export «%s» wrong format varname, 'Aa-Zz 0-9 _'\n", var);
            return (2);
        }
        i++;
    }
	// *length = i;

    return (0);
}

void	ft_adding_to_lists_if_flag(t_data *data, char *new_str)
{
	if (data->ready_create_new_var == 0)
		return ;
	ft_adding_var_to_envplist(data, new_str);
	ft_adding_var_to_sortlist(data, new_str);
	/*если envplist=NULL; ft_lstadd_front() справится?*/
}

void	ft_export(t_data *data, t_list *cmd)
{
	printf("\nEXPORT\n\n");
	char	**tmp_cmd;
	size_t	length;
	int		ret;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return (ft_export_without_args(data));
	while (*tmp_cmd)
	{
		ret = ft_check_varerrors_init_flag(*tmp_cmd, data, &length);
		if (ret == 1)
		{
			ft_change_envplist_if_var_found(*tmp_cmd, length, data);
			ft_change_sortlist_if_var_found(*tmp_cmd, length, data);
			ft_adding_to_lists_if_flag(data, *tmp_cmd);
		}
		else if (ret == 0)
		{
			printf("в переменной нет '='\n");
			// 			ft_change_sortlist_if_var_found(*tmp_cmd, length, data);
			// ft_adding_var_to_sortlist(data, *tmp_cmd);

		}
		tmp_cmd += 1;
	}
}
