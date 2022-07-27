/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:09:41 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/07/25 14:33:20 by mrhyhorn         ###   ########.fr       */
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
	//redirect error to STDERROR
	ft_putstr_fd(str_error, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

char	**ft_strrev_split(char *str, char c)
{
	int		i;
	int		j;
	int		len;
	char	**str_rev;
	char	**splitted;

	splitted = ft_split(str, c);
	i = ft_split_len(splitted);
	str_rev = (char **)malloc(sizeof(char *) * (i + 1));
	if (str_rev == NULL)
		return (NULL);
	len = i;
	j = i;
	i = 0;
	while (i < j)
	{
		str_rev[i++] = ft_strdup(splitted[len - 1]);
		len--;
	}
	str_rev[i] = NULL;
	ft_free_split(splitted);
	return (str_rev);
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