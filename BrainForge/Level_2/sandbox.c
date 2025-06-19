#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;
	int		sig;
	int		code;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		return (-1);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (verbose)
		{
			if (sig == SIGALRM)
				printf("Bad function: timed out after %u seconds\n", timeout);
			else
				printf("Bad function: %s\n", strsignal(sig));
		}
		return (0);
	}
	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		if (code != 0)
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", code);
			return (0);
		}
		if (verbose)
			printf("Nice function!\n");
		return (1);
	}
	return (-1);
}
