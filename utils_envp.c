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
