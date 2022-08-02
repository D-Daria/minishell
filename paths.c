/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 14:05:14 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/02 14:09:54 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_access_paths(t_parser *parser, char *cmd)
{
	int		i;
	char	*join;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	if (parser->paths == NULL || parser->paths[i] == NULL)
		return (NULL);
	while (parser->paths[i])
	{
		printf("parser->paths[i]: %s\n", parser->paths[i]);
		join = ft_strjoin(parser->paths[i], cmd);
		if (access(join, X_OK) == 0)
			return (join);
		else
			free(join);
		i++;
	}
	return (NULL);
}

static void ft_join_path(char **path, t_parser *parser)
{
	char	*join;
	size_t	i;

	i = 0;
	while (path[i])
	{
		join = ft_strjoin(path[i], "/");
		parser->paths[i++] = ft_strdup(join);
		free(join);
	}
	parser->paths[i] = NULL;
}

void	ft_get_paths(t_data *data, t_parser *parser)
{
	char	*env_path;
	char	**path;
	size_t	i;

	env_path = ft_getenv(data, "PATH");
	if (env_path == NULL)
	{
		parser->paths = NULL;
		return ;
	}
	path = ft_split(env_path, ':');
	if (path == NULL)
		return ;
	i = ft_split_len(path);
	parser->paths = (char **)malloc(sizeof(char *) * (i + 1));
	if (parser->paths == NULL)
		return ;
	ft_join_path(path, parser);
	ft_free_split(path);
}
