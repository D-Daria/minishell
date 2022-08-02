#include "minishell.h"

int	ft_is_builtin(t_data *data, t_list *cmd_lst)
{
	char	*cmd;
	int		i;

	i = 0;
	cmd = cmd_lst->cmd_data->cmd[0];
	while (data->builtins[i])
	{
		if ((ft_strcmp(data->builtins[i], cmd)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	ft_start_builtin(t_data *data, t_list *cmd, int i)
{
	ptr_fn_builtin	arr_fn_builtins[7];

	arr_fn_builtins[0] = ft_echo;
	arr_fn_builtins[1] = ft_cd;
	arr_fn_builtins[2] = ft_pwd;
	arr_fn_builtins[3] = ft_export; //Eugene
	arr_fn_builtins[4] = ft_unset; //Eugene
	arr_fn_builtins[5] = ft_env; //Eugene
	arr_fn_builtins[6] = ft_exit;
	arr_fn_builtins[i](data, cmd); /*запускаю функцию с индексом=i,
	передаю ей в параметры (data, cmd)*/
}

void	ft_set_builtins(t_data *data)
{
	data->builtins[0] = "echo";
	data->builtins[1] = "cd";
	data->builtins[2] = "pwd";
	data->builtins[3] = "export";
	data->builtins[4] = "unset";
	data->builtins[5] = "env";
	data->builtins[6] = "exit";
	data->builtins[7] = NULL;
}

int	ft_processing_builtin(t_data *data, t_list *cmd)
{
	int		ret;

	if (!cmd)
		return (-1);
	ret = ft_is_builtin(data, cmd);
	if (ret == -1)
		return (-1);
	ft_start_builtin(data, cmd, ret);
	return (ret);
}