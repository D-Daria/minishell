/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhyhorn <mrhyhorn@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 15:39:15 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/05 23:37:06 by mrhyhorn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* \^ - SIGQUIT */
/* ^C - SIGINT - KILL status - 2 */
/* ^D - EOF */

void	ft_sigquit_handler(int signum)
{
	printf("ft_sigquit_handler\n");
	printf("Quit: %d\n", signum);
}

// void ft_sigquit_process(int signum)
// {
// 	printf("ft_sigquit_process\n");
// 	(void)signum;
// 	exit(signum);
// }

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

void	ft_signals(void)
{
	signal(SIGINT, &ft_sigint_handler);
	signal(SIGQUIT, &ft_sigquit_handler);
}

// void	ft_signals_process(void)
// {
// 	signal(SIGINT, &ft_sigint_handler);
// 	signal(SIGQUIT, &ft_sigquit_process);
// }