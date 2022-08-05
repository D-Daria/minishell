/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:57:00 by sshield           #+#    #+#             */
/*   Updated: 2022/08/05 15:39:39 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_create_envplists(t_data *data, char **envp)
{
	int		i;

	if (!envp)
		ft_error_exit("envp==NULL in ft_create_envplist\n");
	i = 0;
	while (envp[i])
	{
		data->add_new_var_envplist = 1;
		ft_adding_var_to_envplist_if_flag(data, envp[i]);
		data->amount_env_vars += 1;
		data->add_new_var_sortlist = 1;
		ft_adding_var_to_sortlist_if_flag(data, envp[i]);
		i++;
	}
	ft_change_arr_env_vars(data);
}

void	ft_init(t_data *data_ptr, char **envp)
{
	data_ptr->parser_ptr = ft_calloc(1, sizeof(t_parser));
	if (!data_ptr->parser_ptr)
	{
		printf("Malloc error. Minishell has been stopped\n");
		exit(-1);
	}
	ft_create_envplists(data_ptr, envp);
	ft_set_builtins(data_ptr);
}

int main(int argc, char **argv, char **envp)
{
	t_data data;

	(void)argc;
	(void)argv;
	ft_bzero(&data, sizeof(t_data));
	ft_init(&data, envp);
	signal(SIGINT, &ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (data.stopshell == 0)
	{
		ft_read_user_cmd(&data);
		if (data.last_user_cmd == NULL)
			continue;
		ft_parser(&data);
		ft_commands(&data);
		ft_execute(&data);
		ft_free_tokenlist(&data.tokens);
		ft_free_redirs(&data.redirs);
		ft_free_commands(&data.commands);
	}
	ft_free_data_ptr(&data);
	return (0);
}
