/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 21:29:36 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/09 14:39:30 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_data *data, t_list *cmd_lst)
{
	ft_echo_process(data, cmd_lst);
}

void	ft_cd(t_data *data, t_list *cmd)
{
	ft_cd_process(data, cmd);
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
		ft_putendl_fd(path, STDOUT_FILENO);
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
