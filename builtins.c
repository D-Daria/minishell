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
			return (1);
		i++;
	}
	return (0);
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
