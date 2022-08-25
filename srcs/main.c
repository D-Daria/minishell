/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:57:00 by sshield           #+#    #+#             */
/*   Updated: 2022/08/13 14:39:50 by sshield          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	(void)argc;
	ft_bzero(&data, sizeof(t_data));
	ft_init(&data, envp);
	while (1)
	{
		ft_signal(SIGINT, &ft_sigint_handler);
		ft_signal(SIGQUIT, SIG_IGN);
		ft_read_user_cmd(&data);
		ft_signal(SIGINT, &ft_sigint_parent);
		if (data.last_user_cmd == NULL)
			continue ;
		data.exit_status = g_status;
		ft_parser(&data);
		ft_commands(&data);
		ft_execute(&data);
		ft_free_tokenlist(&data.tokens);
		ft_free_redirs(&data.redirs);
		ft_free_commands(&data.commands);
	}
	return (0);
}
