#include <minishell.h>

void    ft_read_user_cmd(t_data *data_ptr)
{
	char	*now_user_cmd;
	char	*rl_buffer;

	rl_buffer = readline(BLUE PROMPT BREAK);
	now_user_cmd = NULL;
	if (rl_buffer == NULL)
	{
		printf("ctr+D:--->exit\n");
		// ft_free_data_ptr(data_ptr);
		exit(0);
	}
	if (rl_buffer && *rl_buffer)
	{
		now_user_cmd = ft_strtrim(rl_buffer, " ");
		if (!now_user_cmd)
		{
			// free (rl_buffer);
			printf("malloc error in ft_read_userstring\n");
			// ft_free_data_ptr(data_ptr);
			exit(-1);
		}
		if (*now_user_cmd == '\0')
		{
			//если введены одни пробелы (после ft_strtrim это пустая строка)
			rl_on_new_line();
			rl_replace_line("", 0);
			data_ptr->last_user_cmd = NULL;
			free(now_user_cmd);
			free(rl_buffer);
			return ;
		}

		if (ft_strcmp(now_user_cmd, data_ptr->last_user_cmd) != 0)
			add_history(now_user_cmd);
		data_ptr->last_user_cmd = ft_strdup(now_user_cmd);
		if (!data_ptr->last_user_cmd)
		{
			// free(now_user_cmd);
			// free (rl_buffer);
			printf("malloc error in ft_read_userstring\n");
			// ft_free_data_ptr(data_ptr);
			exit(-1);
		}
	}
	if (rl_buffer && !(*rl_buffer))
	{
		//если user нажал один только enter(без других символов)
		rl_on_new_line();
		rl_replace_line("", 0);
		data_ptr->last_user_cmd = NULL;
		
	}
	ft_memdel(now_user_cmd);
	// free(now_user_cmd);
	free(rl_buffer);
}
