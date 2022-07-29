/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 21:29:36 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/29 18:51:49 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_builtin(t_data *data, t_list *cmd_lst)
{
	char *cmd;
	int i;

	i = 0;
	cmd = cmd_lst->cmd_data->cmd[0];
	while (data->builtins[i])
	{
		if ((ft_strcmp(data->builtins[i], cmd)) == 0)
			return (1);
		i++;
	}
	return (0);
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

void	ft_execute_builtin(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	// char *cmd_name;

	// cmd_name = cmd->cmd_data->cmd[0];
	// if (!ft_strcmp("echo", cmd_name))
	// 	ft_echo(data);
	// else if (!ft_strcmp("cd", cmd_name))
	// 	ft_cd(data);
	// else if (!ft_strcmp("pwd", cmd_name))
	// 	ft_pwd(data);
	// else if (!ft_strcmp("export", cmd_name)) //Eugene
	// 	ft_export(data);
	// else if (!ft_strcmp("unset", cmd_name)) // unset
	// 	ft_unset(data);
	// else if (!ft_strcmp("env", cmd_name)) //Eugene
	// 	ft_env(data);
	// else if (!ft_strcmp("exit", cmd_name))
	// 	exit(EXIT_SUCCESS);
// 	dup2(*fd_read, STDIN_FILENO);
// 	if (data->parser_ptr->cmds[i + 1] != NULL)
// 		dup2(fd[1], STDOUT_FILENO);
// 	close(fd[0]);
}
