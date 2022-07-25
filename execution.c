
#include "minishell.h"

void	ft_execve(t_data *data, t_list *cmd)
{
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
	ft_perror(cmd);
	exit(0);
}

void	ft_execute_single_cmd(t_data *data, t_list *cmd)
{
	pid_t	pid;
	
	printf("execute single\n");
	pid = fork();
	if (pid < 0)
		exit(ft_throw_system_error("fork"));
	else if (pid == 0)
	{
		printf("parent pid: %d - child pid : %d\n", getppid(), getpid());
		signal(SIGINT, &ft_sigint_handler);//ctr+C передаю в обработчик
		if (cmd->cmd_data->cmd_redir_in || cmd->cmd_data->cmd_redir_out)
			ft_redirect(cmd, data);
		ft_execve(data, cmd);
	}
	else
	{
		waitpid(pid, &data->status, 0);
		printf("execute_single_status: %d\n", WEXITSTATUS(data->status));
	}
}

void	ft_execute_child(t_data *data, t_list **cmd, t_list **prev)
{
	t_list	*cmd_tmp;
	t_list	*prev_tmp;

	cmd_tmp = *cmd;
	prev_tmp = *prev;
	if (prev_tmp)
	{
		dup2(prev_tmp->cmd_data->pipe_fd[0], STDIN_FILENO);
		close(prev_tmp->cmd_data->pipe_fd[0]);
		close(prev_tmp->cmd_data->pipe_fd[1]);
	}
	if (cmd_tmp->next)
	{
		close(cmd_tmp->cmd_data->pipe_fd[0]);
		dup2(cmd_tmp->cmd_data->pipe_fd[1], STDOUT_FILENO);
		close(cmd_tmp->cmd_data->pipe_fd[1]);
	}
	if (cmd_tmp->cmd_data->cmd_redir_in || cmd_tmp->cmd_data->cmd_redir_out)
		ft_redirect(cmd_tmp, data);
	ft_execve(data, cmd_tmp);
}

static void	ft_close_pipes(t_list *prev)
{
	close(prev->cmd_data->pipe_fd[0]);
	close(prev->cmd_data->pipe_fd[1]);
}

void	ft_wait_children(t_data *data)
{
	t_list	*cmd;

	cmd = data->commands;
	while (cmd)
	{
		waitpid(0, &data->status, 0);
		printf("status: %d\n", WEXITSTATUS(data->status));
		cmd = cmd->next;
	}
}

int	ft_pipe(t_data *data, t_list *cmd, t_list *prev, int *pid)
{
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(cmd->cmd_data->pipe_fd) < 0)
				return (ft_throw_system_error("pipe"));
		}
		*pid = fork();
		if (*pid < 0)
			return (ft_throw_system_error("fork"));
		else if (*pid == 0)
			ft_execute_child(data, &cmd, &prev);
		else
		{
			if (prev)
				ft_close_pipes(prev);
			prev = cmd;
			cmd = cmd->next;
		}
	}
	ft_wait_children(data);
	return (0);
}

void	ft_execute(t_data *data)
{
	t_list	*cmd;
	t_list	*prev;
	int		pid;

	cmd = data->commands;
	prev = NULL;
	if (data->redirs)
		ft_process_redirs(data);
	debug_print_commands_list(data);
	if (cmd && data->pipes_number > 0)
		ft_pipe(data, cmd, prev, &pid);
	else
		ft_execute_single_cmd(data, data->commands);
	ft_close_all(data, NULL);
}
