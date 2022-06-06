/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarcele <mmarcele@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 18:30:15 by ghelman           #+#    #+#             */
/*   Updated: 2022/03/13 12:25:33 by mmarcele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

static int	g_received;

static void	handler(int number, siginfo_t *info, void *ucontext)
{
	(void)number;
	(void)info;
	(void)ucontext;
	g_received = 1;
}

static pid_t	get_pid_integer(char *str)
{
	pid_t	pid;

	pid = 0;
	while (*str)
		pid = pid * 10 + *str++ - '0';
	return (pid);
}

static size_t	get_msg_length(char *str)
{
	size_t	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

static void	message(char *msg, size_t length, pid_t pid)
{
	size_t	i;
	size_t	j;
	int		bit;
	int		signal;

	i = 0;
	while (i < length)
	{
		j = 0;
		while (j < 8)
		{
			bit = msg[i] >> j & 1;
			g_received = 0;
			if (bit % 2 == 0)
				signal = SIGUSR1;
			else
				signal = SIGUSR2;
			while (kill(pid, signal) != 0)
				;
			while (!g_received)
				;
			j++;
		}
		i++;
	}
}

int	main(int argc, char *argv[])
{
	struct sigaction	sig_act;
	pid_t				pid;
	char				*msg;

	if (argc != 3)
		return (1);
	pid = (pid_t)get_pid_integer(argv[1]);
	msg = argv[2];
	sig_act.sa_flags = SA_SIGINFO;
	sig_act.__sigaction_u.__sa_sigaction = handler;
	while (sigemptyset(&sig_act.sa_mask) != 0)
		;
	while (sigaction(SIGUSR1, &sig_act, NULL))
		;
	message(msg, get_msg_length(msg), pid);
}
