/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 14:10:58 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/09 00:58:30 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_nl(char **cmd, int *j, int *nl)
{
	size_t	i;
	char	*str;

	i = 0;
	while (cmd[*j])
	{
		str = ft_strnstr(cmd[*j], "-n", ft_strlen("-n"));
		if (str)
		{
			i = 1;
			while (str[i])
			{
				if (str[i] != 'n')
					return ;
				i++;
			}
			*nl = 0;
		}
		else
			return ;
		*j += 1;
	}
}

void	ft_echo_process(t_data *data, t_list *cmd_lst)
{
	size_t	i;
	size_t	len;
	int		j;
	int		nl;
	char	**cmd;

	(void)data;
	cmd = cmd_lst->cmd_data->cmd;
	nl = 1;
	j = 1;
	ft_nl(cmd, &j, &nl);
	i = j;
	len = ft_split_len(cmd);
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], STDOUT_FILENO);
		if (i != len - 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (nl)
		ft_putstr_fd("\n", STDOUT_FILENO);
}
