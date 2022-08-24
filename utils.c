/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:09:41 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/28 22:26:18 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_throw_system_error(const char *str)
{
	perror(str);
	return (-1);
}

void	ft_error_exit(const char *str_error)
{
	if (str_error == NULL)
		exit(-1);
	ft_putstr_fd(str_error, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

size_t	ft_split_len(char **str)
{
	int	i;

	if (str == NULL)
		return (-1);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	ft_check_in_change_envplist(t_data *data, char *new_var, char ch)
{
	if (!new_var)
		ft_error_exit("malloc error\n");
	if (ch != '=')
	{
		ft_delete_if_found_in_envplist(&new_var, data);
		ft_memdel(new_var);
		data->add_new_var_envplist = 0;
		return (1);
	}
	return (0);
}
