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
			return (i);
		i++;
	}
	return (-1);
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

void	ft_echo(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf(" ECHO\n\n");
}

void	ft_cd(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf(" CD\n\n");
}

void	ft_pwd(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;

	printf(" PWD\n\n");
}

void	ft_export_without_args(t_data *data)
{
	printf("\nNO args\n\n");
	/*sort,print envplist*/
}

void	ft_parse_cmd_to_find_env_var()
{

}

void	ft_get_length_init_flag(char *cmd, size_t *length, t_data *data)
{
	data->flag = 0;
	*length = 0;
	while (cmd[*length] != '=' && cmd[*length])
		*length +=1;
}


void	ft_check_flag_add_envplist(t_data *data, char *new_str)
{
	t_list	*new;

	if (data->flag == 1)
		return ;
	new = ft_calloc(1, sizeof(t_list));//check calloc
	new->envp_str = ft_strdup(new_str);//check malloc
	ft_lstadd_front(&data->envplist, new);
		printf("ADD_LIST\n");

	/*если envplist=NULL; ft_lstadd_front() справится?*/
}

void	ft_change_var_change_flag(t_list **current_env, char *new, t_data *data)
{
	char	*old;

	data->flag = 1;
	old = (*current_env)->envp_str;
	(*current_env)->envp_str = ft_strdup(new);//malloc
	free (old);
	printf("CHANGE\n");
}

void	ft_export(t_data *data, t_list *cmd)
{
	printf("\nEXPORT\n\n");
	/*check, var не должна начинаться с цифры, ещё...
	сорт по первой букве, сначала большие, потом мелкие,
	как по номеру в ascii
	В declare_список экспортируются 
	переменные без знака '=' (после команды export z, перем z войдёт в 
	declare_список, и не войдёт в список из команды env,
	позже, после команды z=123, изменяются оба списка!)
	в то же время, после команды export z=, переменная z=''войдет в оба.
	т.е.
	сначала заполнять declare_список(сортировать), а когда надо, из него
	переносить переменные и в env_список*/
	char	**tmp_cmd;
	size_t	length;
	t_list	*current_env;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return (ft_export_without_args(data));
	while (*tmp_cmd)
	{
		//ft_parse_cmd_check_errors(); tmp_cmd += 1; break ; (при ошибке)
		if (ft_strchr(*tmp_cmd, (int)'=') == NULL)
		{
			//ft_add_to_declare_list();
			tmp_cmd += 1;
			continue ;
		}
		ft_get_length_init_flag(*tmp_cmd, &length, data);
		current_env = data->envplist;
		while (current_env)
		{
			if (ft_strncmp(current_env->envp_str, *tmp_cmd, length) == 0 \
				&& ((current_env->envp_str)[length] == '=' ))
			{
				ft_change_var_change_flag(&current_env, *tmp_cmd, data);
				break ;
			}
			current_env = current_env->next;
		}
		ft_check_flag_add_envplist(data, *tmp_cmd);
		//check_flag_add_to_declare_list()
		tmp_cmd += 1;
	}
}

void	ft_delete_env_var(t_data *data, t_list **prev, t_list **var)
{
	if ((*prev) == NULL)//переменная нашлась в первой строке, удалить этот list
		data->envplist = (*var)->next;
	else 
		(*prev)->next = (*var)->next;
	free ((*var)->envp_str);
	free (*var);
}

// void	ft_unset(t_data *data, t_list *cmd)
// {
// 	char	**tmp_cmd;
// 	t_list	*current_env;
// 	t_list	*prev_env;

// 	tmp_cmd = (cmd->cmd_data->cmd) + 1;
// 	if (!(*tmp_cmd))
// 		return ;
// 	while (*tmp_cmd)
// 	{
// 		current_env = data->envplist;
// 		prev_env = NULL;
// 		while (current_env)
// 		{
// 			if (ft_strncmp(current_env->envp_str, *tmp_cmd, \
// 				ft_strlen(*tmp_cmd)) == 0 && \
// 				((current_env->envp_str)[ft_strlen(*tmp_cmd)] == '=' \
// 				|| (current_env->envp_str)[ft_strlen(*tmp_cmd)] == '\0'))
// 			{
// 				ft_delete_env_var(data, &prev_env, &current_env);
// 				break ;
// 			}
// 			prev_env = current_env;
// 			current_env = current_env->next;
// 		}
// 		tmp_cmd += 1;
// 	}
// }

void	ft_unset(t_data *data, t_list *cmd)
{
	char	**tmp_cmd;
	t_list	*current_env;
	t_list	*prev_env;
	size_t	length;

	tmp_cmd = (cmd->cmd_data->cmd) + 1;
	if (!(*tmp_cmd))
		return ;
	while (*tmp_cmd)
	{
		//ft_parse_cmd_check_errors(); tmp_cmd += 1; break ; (при ошибке)
		current_env = data->envplist;
		prev_env = NULL;
		while (current_env)
		{
			ft_get_length_init_flag(current_env->envp_str, &length, data);
			if (ft_strncmp(current_env->envp_str, *tmp_cmd, length) == 0 \
				&& ((*tmp_cmd)[length] == '\0'))
			{
				ft_delete_env_var(data, &prev_env, &current_env);
				//ft_delete_var_in_declare_list();
				break ;
			}
			prev_env = current_env;
			current_env = current_env->next;
		}
		tmp_cmd += 1;
	}
}

void	ft_env(t_data *data, t_list *cmd)
{
	t_list	*ptr;

	if (!data->envplist || !cmd)
		return ;
	ptr = data->envplist;
	while (ptr)
	{
		printf("%s\n", ptr->envp_str);
		ptr = ptr->next;
	}
}

void	ft_exit(t_data *data, t_list *cmd)
{
	(void)data;
	(void)cmd;
	printf("\nEXIT\n\n");
}

void	ft_start_builtin(t_data *data, t_list *cmd, int i)
{
	ptr_fn_builtin	arr_fn_builtins[7];

	arr_fn_builtins[0] = ft_echo;
	arr_fn_builtins[1] = ft_cd;
	arr_fn_builtins[2] = ft_pwd;
	arr_fn_builtins[3] = ft_export;
	arr_fn_builtins[4] = ft_unset;
	arr_fn_builtins[5] = ft_env;
	arr_fn_builtins[6] = ft_exit;
	arr_fn_builtins[i](data, cmd);/*запускаю функцию с индексом=i,
	передаю ей в параметры (data, cmd)*/
}

int	ft_processing_builtin(t_data *data, t_list *cmd)
{
	int		ret;

	if (!cmd)
		return (-1);
	ret = ft_is_builtin(data, *cmd->cmd_data->cmd);
	if (ret == -1)
		return (-1);
	ft_start_builtin(data, cmd,ret);
	return (ret);
}