/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 14:19:25 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/10 16:09:51 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_validate_digits(t_list *cmd)
{
	int		i;
	char	*args;

	i = 0;
	args = cmd->cmd_data->cmd[1];
	while (args[i])
	{
		if (args[i] == '-' || ft_isdigit(args[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

void	ft_exit_process(t_data *data, t_list *cmd)
{
	size_t	len;

	len = ft_split_len(cmd->cmd_data->cmd);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (len > 2)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putendl_fd("too many arguments", STDERR_FILENO);
		data->status = 1;
		return ;
	}
	else if (len == 2)
	{
		if (!ft_validate_digits(cmd))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_data->cmd[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit(255);
		}
		exit(ft_atoi(cmd->cmd_data->cmd[1]));
	}
	exit(data->exit_status);
}