/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 21:29:36 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/08 14:14:35 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_data *data, t_list *cmd_lst)
{
	ft_echo_process(data, cmd_lst);
	// size_t	i;
	// size_t	len;
	// int		nl;
	// char	**cmd;

	// (void)data;
	// cmd = cmd_lst->cmd_data->cmd;
	// nl = 1;
	// i = 1;
	// if (ft_strcmp(cmd[1], "-n") == 0)
	// {
	// 	i = 2;
	// 	nl = 0;
	// }
	// len = ft_split_len(cmd);
	// while (cmd[i])
	// {
	// 	ft_putstr_fd(cmd[i], STDOUT_FILENO);
	// 	if (i != len - 1)
	// 		ft_putstr_fd(" ", STDOUT_FILENO);
	// 	i++;
	// }
	// if (nl)
	// 	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	ft_cd(t_data *data, t_list *cmd)
{
	char	*arg;
	char	*home;

	home = ft_getenv(data, "HOME");
	arg = cmd->cmd_data->cmd[1];
	if (arg)
	{
		if (ft_strcmp(arg, "~") == 0)
			arg = home;
		if (chdir(arg) == -1 && arg)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(arg);
			data->status = 1;
		}
	}
	else if (!arg)
	{
		if (chdir(home) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			data->status = 1;
		}
	}
}

void	ft_pwd(t_data *data, t_list *cmd)
{
	char	*path;

	(void)data;
	(void)cmd;
	path = NULL;
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("getcwd()");
		data->status = 1;
	}
	else
	{
		printf("%s\n", path);
		free(path);
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
	printf("exit\n");
	if (cmd->next)
		exit(EXIT_FAILURE);
	else
		exit(0);
}
