#include <minishell.h>

char    *ft_getenv(t_data *data, char *var)
{
    t_list  *tmp;
    char    *ret_str;

    if (data->envplist != NULL)
    {
        tmp = data->envplist;
        while (tmp)
        {
            if (ft_strncmp(tmp->envp_str, var, ft_strlen(var)) == 0 \
                && (tmp->envp_str)[ft_strlen(var)] == '=')
            {
                ret_str = tmp->envp_str + ft_strlen(var) + 1;
                // ret_str = ft_substr(tmp->envp_str, ft_strlen(var) + 1, \
                //     ft_strlen(tmp->envp_str) - (ft_strlen(var) + 1));
                return (ret_str);
            }
            tmp = tmp->next;
        }
    }
    return (NULL);
}
