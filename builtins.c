/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 21:29:36 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/30 14:15:10 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_data *data, t_list *cmd_lst)
{
	size_t	i;
	size_t	len;
	int		nl;
	char	**cmd;

	(void)data;
	cmd = cmd_lst->cmd_data->cmd;
	printf(" ECHO\n");
	nl = 1;
	i = 1;
	if (ft_strcmp(cmd[1], "-n") == 0)
	{
		i = 2;
		nl = 0;
	}
	len = ft_split_len(cmd);
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (i != len - 1)
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
}

void	ft_cd(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	printf(" CD\n\n");
	printf("dir before: %s\n", getcwd(NULL, 0));
	if (cmd->cmd_data->cmd[1])
	{
		if (chdir(cmd->cmd_data->cmd[1]) == -1)
			perror("chdir");
		else
			printf("dir after: %s\n", getcwd(NULL, 0));
	}
}

void	ft_pwd(t_data *data, t_list *cmd)
{
	char	*path;

	(void)data;
	(void)cmd;
	printf(" PWD\n\n");
	path = NULL;
	path = getcwd(NULL, 0);
	if (path == NULL)
		perror("getcwd()");
	else
	{
		printf("%s\n", path);
		free(path);
	}
}

void	ft_export(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	printf(" EXPORT\n\n");
}

void	ft_unset(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	printf(" UNSET\n\n");
}

void	ft_env(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
}

void	ft_exit(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	printf("exit\n");
	if (cmd->next)
		exit(EXIT_FAILURE);
	else
		exit(127);
}

int	ft_processing_builtin(t_data *data, t_list *cmd)
{
	int		ret;

	if (!cmd)
		return (-1);
	ret = ft_is_builtin(data, cmd);
	if (ret == -1)
		return (-1);
	printf("\nBUILTIN\n");
	ft_start_builtin(data, cmd, ret);
	return (ret);
}
