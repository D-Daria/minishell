/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 19:23:12 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/02 13:39:41 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_split(char **split_string)
{
	int	i;

	i = 0;
	if (split_string)
	{
		while (split_string[i] != NULL)
		{
			ft_memdel(split_string[i]);
			i++;
		}
		ft_memdel(split_string);
	}
	else
		return ;
}
