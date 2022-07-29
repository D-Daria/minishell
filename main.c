/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:57:00 by sshield           #+#    #+#             */
/*   Updated: 2022/07/24 13:58:32 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_find_path_by_launch(char **envp, t_data *data_ptr)
// {
// 	int i;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		if ((ft_strncmp(envp[i], "PATH=", 5)) == 0)
// 		{
// 			data_ptr->path_by_launch = ft_split((envp[i] + 5), ':');
// 			if (!data_ptr->path_by_launch)
// 			{
// 				ft_free_data_ptr(data_ptr);
// 				printf("Malloc error in ft_find_path_by_launch\n");
// 				exit(-1);
// 			}
// 			return ;
// 		}
// 		i++;
// 	}
// }

void	ft_create_envplist(t_data *data_ptr, char **envp)
{
	t_list	*new;
	int		i;

	if (!envp)
		ft_error_exit("envp==NULL in ft_create_envplist\n");

	i = 0;
	while (envp[i])
	{
		new = ft_calloc(1, sizeof(t_list));//check calloc
		new->envp_str = ft_strdup(envp[i]);//check malloc
		ft_lstadd_back(&data_ptr->envplist, new);
		i++;
	}
}

void	ft_init(t_data *data_ptr, char **envp)
{
	data_ptr->parser_ptr = ft_calloc(1, sizeof(t_parser));
	if (!data_ptr->parser_ptr)
	{
		printf("Malloc error. Minishell has been stopped\n");
		exit(-1);
	}
	ft_create_envplist(data_ptr, envp);
	ft_set_builtins(data_ptr);
	data_ptr->envp = envp;
	// ft_find_path_by_launch(envp, data_ptr);
}

void	ft_sigint_handler(int signum)
{
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 0);
	rl_redisplay();
	(void)signum;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	ft_bzero(&data, sizeof(t_data));
	ft_init(&data, envp);
	printf("ft_getenv=%s\n", ft_getenv(&data, "TERM"));
	signal(SIGINT, &ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (data.stopshell == 0)
	{
		ft_read_user_cmd(&data);
		if (data.last_user_cmd == NULL)
			continue ;
		ft_parser(&data);
		ft_commands(&data);
		ft_execute(&data);
		ft_free_list(&data.tokens);
		ft_free_redirs(&data.redirs);
		ft_free_commands(&data.commands);
	}
	// ft_free_data_ptr(&data);
	return (0);
}
