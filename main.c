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

// void	ft_create_envplist(t_data *data_ptr, char **envp)
// {
// 	t_list	*new;
// 	int		i;

// 	if (!envp)
// 		ft_error_exit("envp==NULL in ft_create_envplist\n");

// 	i = 0;
// 	while (envp[i])
// 	{
// 		new = ft_calloc(1, sizeof(t_list));//check calloc
// 		new->envp_str = ft_strdup(envp[i]);//check malloc
// 		ft_lstadd_back(&data_ptr->envplist, new);
// 		i++;
// 	}
// }

void	ft_add_to_sorted_list(t_data *d, t_list **prev, char *s_new)
{
	t_list	*l_new;
	t_list	*tmp;

	l_new = ft_calloc(1, sizeof(t_list));
	l_new->envp_str = ft_strdup(s_new);
	//-------------------------
	if (!(*prev))
	{
		ft_lstadd_front(&(d->sorted_envplist), l_new);
		return ;
	}
	tmp = (*prev)->next;
	(*prev)->next = l_new;
	l_new->next = tmp;
}

void	ft_create_envplist(t_data *data_ptr, char **envp)
{
	t_list	*new;
	t_list	*current;
	t_list	*prev;
	int		i;

	if (!envp)
		ft_error_exit("envp==NULL in ft_create_envplist\n");

	i = 0;
	while (envp[i])
	{
		new = ft_calloc(1, sizeof(t_list));//check calloc
		new->envp_str = ft_strdup(envp[i]);//check malloc
		ft_lstadd_back(&data_ptr->envplist, new);
		//-----------------------------------
		current = data_ptr->sorted_envplist;
		prev = NULL;
		while (current)
		{
			if (ft_strcmp(envp[i], current->envp_str) < 0)
			{
				break ;
			}
			prev = current;
			current = current->next;
		}
		ft_add_to_sorted_list(data_ptr, &prev, envp[i]);
		i++;
	}
}

// void	ft_create_sorted_envplist(t_data *data)
// {
// 	t_list	*sort;
// 	t_list	*env;

// 	sort = data->sorted_envplist;
// 	env = data->envplist;
// 	while (env)
// 	{
// 		//
// 		ft_calloc(1, sizeof(t_list));
// 		while (sort)
// 		{
// 			if ()
// 			{
// 				/* code */
// 			}
			
// 			sort = sort->next;
// 		}
// 		env = env->next;
// 	}
// }

void	ft_init(t_data *data_ptr, char **envp)
{
	data_ptr->parser_ptr = ft_calloc(1, sizeof(t_parser));
	if (!data_ptr->parser_ptr)
	{
		printf("Malloc error. Minishell has been stopped\n");
		exit(-1);
	}
	ft_create_envplist(data_ptr, envp);
	// ft_create_sorted_envplist(data_ptr);
	ft_set_builtins(data_ptr);
	data_ptr->envp = envp;
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
			printf("getenv(q)=%s\n", getenv("q"));
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
