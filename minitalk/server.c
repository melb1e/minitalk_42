/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarcele <mmarcele@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 18:32:15 by ghelman           #+#    #+#             */
/*   Updated: 2022/03/13 12:20:16 by mmarcele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void	handler(int sig, siginfo_t *info, void *ucontext)
{
	static int				i;
	static unsigned char	c;
	static int				pid;
	int						bit;
	int						crutch;

	(void)ucontext;
	if (sig == SIGUSR1)
		bit = 0;
	else
		bit = 1;
	c = c | bit << i;
	if (i == 8 - 1)
	{
		write(1, &c, 1);
		c = 0;
	}
	i = (i + 1) % 8;
	if (info->si_pid != 0)
		pid = info->si_pid;
	crutch = -1;
	while (crutch != 0)
		crutch = kill(pid, SIGUSR1);
}

static size_t	get_pid_length(pid_t pid)
{
	size_t	length;

	length = 1;
	if (pid < 0)
	{
		pid = -pid;
		length++;
	}
	while (pid > 9)
	{
		pid /= 10;
		length++;
	}
	return (length);
}

static char	*get_pid_string(pid_t pid)
{
	char	*str;
	size_t	length;

	length = get_pid_length(pid);
	str = malloc(sizeof(char) * (length + 1));
	if (str == NULL)
		return (NULL);
	str[length] = '\0';
	while (length)
	{
		str[length - 1] = pid % 10 + '0';
		pid /= 10;
		length--;
	}
	return (str);
}

static void	print_pid(pid_t pid)
{
	size_t	length;
	char	*str;

	str = get_pid_string(pid);
	length = get_pid_length(pid);
	write(1, "The ID of this process/group of processes: ", 43);
	write(1, str, length);
	free(str);
}

int	main(void)
{
	struct sigaction	sig_act;

	sig_act.sa_flags = SA_SIGINFO;
	sig_act.__sigaction_u.__sa_sigaction = handler;
	sigemptyset(&sig_act.sa_mask);
	sigaction(SIGUSR1, &sig_act, NULL);
	sigaction(SIGUSR2, &sig_act, NULL);
	print_pid(getpid());
	while (write(1, "\n", 1) != 1)
		;
	while (1)
		pause();
}
