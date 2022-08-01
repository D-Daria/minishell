#include "minishell.h"

void	ft_get_status(t_data *data)
{
	printf("exit status: %d\n", WEXITSTATUS(data->status));
	printf("stop status: %d\n", WSTOPSIG(data->status));
	printf("if signaled: %d\n", WIFSIGNALED(data->status));
	if (WIFSIGNALED(data->status))
		data->status = 128 + SIGINT;
	else
		data->status = WEXITSTATUS(data->status);
}

void	ft_execve(t_data *data, t_list *cmd)
{
	if (access(cmd->cmd_data->cmd_path, X_OK) == 0)
	{
		execve(cmd->cmd_data->cmd_path, cmd->cmd_data->cmd, data->envp);
		ft_perror(cmd);
	}
	ft_perror(cmd);
}

void	ft_execute_single_cmd(t_data *data, t_list *cmd)
{
	pid_t	pid;

	if (ft_processing_builtin(data, cmd) >= 0)
		return ;
	pid = fork();
	if (pid < 0)
		exit(ft_throw_system_error("fork"));
	else if (pid == 0)
	{
		signal(SIGINT, &ft_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		if (cmd->cmd_data->is_redir)
			ft_redirect(cmd, data);
		ft_execve(data, cmd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &data->status, 0);
		ft_get_status(data);
	}
}

void	ft_execute_child(t_data *data, t_list **cmd, t_list **prev)
{
	signal(SIGINT, &ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if ((*prev))
	{
		dup2((*prev)->cmd_data->pipe_fd[0], STDIN_FILENO);
		close((*prev)->cmd_data->pipe_fd[0]);
		close((*prev)->cmd_data->pipe_fd[1]);
	}
	if ((*cmd)->next)
	{
		close((*cmd)->cmd_data->pipe_fd[0]);
		dup2((*cmd)->cmd_data->pipe_fd[1], STDOUT_FILENO);
		close((*cmd)->cmd_data->pipe_fd[1]);
	}
	if (!(*prev) && !(*cmd)->next) //в случае > file | wc -l
	{
		dup2((*cmd)->cmd_data->pipe_fd[0], STDIN_FILENO);
		close((*cmd)->cmd_data->pipe_fd[0]);
		close((*cmd)->cmd_data->pipe_fd[1]);
	}
	if ((*cmd)->cmd_data->is_redir)
		ft_redirect((*cmd), data);
	if (ft_processing_builtin(data, (*cmd)) == -1)
		ft_execve(data, (*cmd));
	exit(EXIT_SUCCESS);
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
		ft_get_status(data);
		cmd = cmd->next;
	}
	ft_close_all(data);
}

int	ft_pipe(t_data *data, t_list *cmd, t_list *prev, int *pid)
{
	while (cmd)
	{
		if (pipe(cmd->cmd_data->pipe_fd) < 0)
			return (ft_throw_system_error("pipe"));
		*pid = fork();
		if (*pid < 0)
			return (ft_throw_system_error("fork"));
		else if (*pid == 0)
			ft_execute_child(data, &cmd, &prev);
		else
		{
			if (prev)
				ft_close_pipes(prev);
			if (!cmd->next)
				ft_close_pipes(cmd);
			if (access("here_doc", F_OK) == 0)
				unlink("here_doc");
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

	data->status = 0;
	cmd = data->commands;
	prev = NULL;
	if (data->redirs)
		ft_process_redirs(data);
	ft_set_builtins(data);
	debug_print_commands_list(data);
	if (data->status > 0)
		return ;
	if (cmd && data->pipes_number > 0)
		ft_pipe(data, cmd, prev, &pid);
	else if (cmd)
		ft_execute_single_cmd(data, data->commands);
}
