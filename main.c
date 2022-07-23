/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:57:00 by sshield           #+#    #+#             */
/*   Updated: 2022/07/20 16:37:51 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_find_path_by_launch(char **envp, t_data *data_ptr)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if ((ft_strncmp(envp[i], "PATH=", 5)) == 0)
		{
			data_ptr->path_by_launch = ft_split((envp[i] + 5), ':');
			if (!data_ptr->path_by_launch)
			{
				ft_free_data_ptr(data_ptr);
				printf("Malloc error in ft_find_path_by_launch\n");
				exit(-1);
			}
			return ;
		}
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
	data_ptr->envp = envp;
	ft_find_path_by_launch(envp, data_ptr);
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
	signal(SIGINT, &ft_sigint_handler);//ctr+C передаю в обработчик
	signal(SIGQUIT, SIG_IGN);//ctr+backslash игнорирую
	while (data.stopshell == 0)
	{
		ft_read_user_cmd(&data);
		if (data.last_user_cmd == NULL)
			continue ;
		ft_parser(&data);
		ft_commands(&data);
		ft_execute(&data);
		ft_free_list(&data.tokens);
		ft_free_commands(&data.commands);
		printf("pipes=%zu\n", data.pipes_number);
	}
	ft_free_data_ptr(&data);
	return (0);
}
