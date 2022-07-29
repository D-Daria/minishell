/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 21:29:36 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/20 16:10:00 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_builtin(t_data *data, char *cmd)
{
	int	i;

	i = 0;
	while (data->builtins[i])
	{
		if ((ft_strcmp(data->builtins[i], cmd)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	ft_set_builtins(t_data *parser)
{
	parser->builtins[0] = "echo";
	parser->builtins[1] = "cd";
	parser->builtins[2] = "pwd";
	parser->builtins[3] = "export";
	parser->builtins[4] = "unset";
	parser->builtins[5] = "env";
	parser->builtins[6] = "exit";
	parser->builtins[7] = NULL;
}

void	ft_echo(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf(" ECHO\n\n");
}

void	ft_cd(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf(" CD\n\n");
}

void	ft_pwd(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf(" PWD\n\n");
}

void	ft_export(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf("EXPORT\n\n");
}

void	ft_delete_env_var(t_data *data, t_list **prev, t_list **var)
{
	if ((*prev) == NULL)//переменная нашлась в первой строке, удалить этот list
		data->envplist = (*var)->next;
	else 
		(*prev)->next = (*var)->next;
	free ((*var)->envp_str);
	free (*var);
}

void	ft_unset(t_data *data, t_list *cmd)
{
	char	**tmp_cmd;
	t_list	*current_env;
	t_list	*prev_env;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return ;
	while (*tmp_cmd)
	{
		current_env = data->envplist;
		prev_env = NULL;
		while (current_env)
		{
			if (ft_strncmp(current_env->envp_str, *tmp_cmd, \
				ft_strlen(*tmp_cmd)) == 0 && \
				(current_env->envp_str)[ft_strlen(*tmp_cmd)] == '=')
			{
				ft_delete_env_var(data, &prev_env, &current_env);
				break ;
			}
			prev_env = current_env;
			current_env = current_env->next;
		}
		tmp_cmd += 1;
	}
}

void	ft_env(t_data *data, t_list *cmd)
{
	t_list	*ptr;

	if (!data->envplist || !cmd)
		return ;
	ptr = data->envplist;
	while (ptr)
	{
		printf("%s\n", ptr->envp_str);
		ptr = ptr->next;
	}
}

void	ft_exit(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	printf("\nEXIT\n\n");
}

void	ft_start_builtin(t_data *data, t_list *cmd, int i)
{
	ptr_fn_builtin	arr_fn_builtins[7];

	arr_fn_builtins[0] = ft_echo;
	arr_fn_builtins[1] = ft_cd;
	arr_fn_builtins[2] = ft_pwd;
	arr_fn_builtins[3] = ft_export;
	arr_fn_builtins[4] = ft_unset;
	arr_fn_builtins[5] = ft_env;
	arr_fn_builtins[6] = ft_exit;
	arr_fn_builtins[i](data, cmd);/*запускаю функцию с индексом=i,
	передаю ей в параметры (data, cmd)*/
}

int	ft_processing_builtin(t_data *data, t_list *cmd)
{
	int		ret;

	if (!cmd)
		return (-1);
	ret = ft_is_builtin(data, *cmd->cmd_data->cmd);
	if (ret == -1)
		return (-1);
	ft_start_builtin(data, cmd,ret);
	return (ret);
}