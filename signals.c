/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 15:39:15 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/07 16:18:55 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* \^ - SIGQUIT */
/* ^C - SIGINT - KILL status - 2 */
/* ^D - EOF */

void	ft_sigint_parent(int signum)
{
	(void)signum;
}

void	ft_sigquit_parent(int signum)
{
	size_t	input;
	char	buff[1024];

	input = 0;
	input = read(1, buff, STDIN_FILENO);
	printf("input: %zu\n", input);
	if (input <= 0)
		close(STDIN_FILENO);
	(void)signum;
}

void ft_sigquit_child(int signum)
{
	if (signum == SIGQUIT)
		exit(signum);
}

void	ft_sigint_handler(int signum)
{
	t_termios	term;
	int			res;

	res = tcgetattr(0, &term); /* чтобы не отображался ^C */
	if (res < 0)
	{
		ft_throw_system_error("tcgetattr");
		return;
	}
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
	rl_on_new_line();
	printf("\n");
	rl_replace_line("", 0);
	rl_redisplay();
	(void)signum;
}

void	ft_signals_child(void)
{
	signal(SIGQUIT, &ft_sigquit_child);
	signal(SIGINT, SIG_DFL);
}
