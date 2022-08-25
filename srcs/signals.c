/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshield <sshield@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 15:39:15 by mrhyhorn          #+#    #+#             */
/*   Updated: 2022/08/13 14:29:28 by sshield          ###   ########.fr       */
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

void	ft_sigquit_child(int signum)
{
	if (signum == SIGQUIT)
		exit(signum);
}

void	ft_sigint_handler(int signum)
{
	t_termios	term;
	int			res;

	res = tcgetattr(0, &term);
	if (res < 0)
	{
		ft_throw_system_error("tcgetattr");
		return ;
	}
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
	rl_on_new_line();
	ft_putstr_fd("\n", STDIN_FILENO);
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 1;
	(void)signum;
}

void	ft_signals_child(void)
{
	ft_signal(SIGQUIT, &ft_sigquit_child);
	ft_signal(SIGINT, SIG_DFL);
}

int	ft_signal(const int signum, void f(int))
{
	t_sig	sig;

	ft_memset(&sig, 0, sizeof(sig));
	sigemptyset(&sig.sa_mask);
	sig.sa_handler = f;
	sig.sa_flags = SA_RESTART;
	if (sigaction(signum, &sig, NULL) == -1)
		return (ft_throw_system_error("minishell: catch signal failed\n"));
	return (0);
}
