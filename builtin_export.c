#include "minishell.h"

void	ft_export_without_args(t_data *data)
{
	t_list	*current;
	size_t	i;

	printf("NO args\n\n");
	current = data->sorted_envplist;
	while (current)
	{
		printf("declare x ", current->envp_str);
		i = 0;
		while ((current->envp_str)[i] != '=')
			printf("%c", (current->envp_str)[i++]);
		printf("=\"%s\"\n", current->envp_str + i + 1);			
		current = current->next;
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
    printf("в переменной нет '='\n");
    return (0);
}

void	ft_change_var(t_list **current_env, char *new, t_data *data)
{
	char	*old;

	data->ready_create_new_var = 0;
	old = (*current_env)->envp_str;
	(*current_env)->envp_str = ft_strdup(new);//malloc
	free (old);
	printf("CHANGE\n");
}

void    ft_find_env_var(char *var, size_t l, t_data *data)
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
                printf("var уже в списке envplist, делаю return\n");
                data->ready_create_new_var = 0;
                return ;
            }
	    	ft_change_var(&env, var, data);
			break ;
		}
		env = env->next;
	}
}

void	ft_check_flag_add_envplist(t_data *data, char *new_str)
{
	t_list	*new;

	if (data->ready_create_new_var == 0)
		return ;
	new = ft_calloc(1, sizeof(t_list));//check calloc
	new->envp_str = ft_strdup(new_str);//check malloc
	ft_lstadd_front(&data->envplist, new);
		printf("ADD_LIST\n");

	/*если envplist=NULL; ft_lstadd_front() справится?*/
}

void	ft_export(t_data *data, t_list *cmd)
{
	printf("\nEXPORT\n\n");
	/*check, var не должна начинаться с цифры, ещё...
	сорт по первой букве, сначала большие, потом мелкие,
	как по номеру в ascii
	В declare_список экспортируются 
	переменные без знака '=' (после команды export z, перем z войдёт в 
	declare_список, и не войдёт в список из команды env,
	позже, после команды z=123, изменяются оба списка!)
	в то же время, после команды export z=, переменная z=''войдет в оба.
	т.е.
	сначала заполнять declare_список(сортировать), а когда надо, из него
	переносить переменные и в env_список*/
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
			ft_find_env_var(*tmp_cmd, length, data);
			ft_check_flag_add_envplist(data, *tmp_cmd);
		}
		tmp_cmd += 1;
	}
}
