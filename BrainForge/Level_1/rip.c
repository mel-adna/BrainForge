#include <unistd.h>

void	solve(char *s, int i, int open, int rm, char *tmp)
{
	if (!s[i])
	{
		if (open == 0)
		{
			tmp[i] = '\0';
			write(1, tmp, i);
			write(1, " ", 1);
			write(1, "\n", 1);
		}
		return ;
	}
	if (s[i] == '(')
	{
		tmp[i] = '(';
		solve(s, i + 1, open + 1, rm, tmp);
		if (rm > 0)
		{
			tmp[i] = ' ';
			solve(s, i + 1, open, rm - 1, tmp);
		}
	}
	else if (s[i] == ')')
	{
		if (open > 0)
		{
			tmp[i] = ')';
			solve(s, i + 1, open - 1, rm, tmp);
		}
		if (rm > 0)
		{
			tmp[i] = ' ';
			solve(s, i + 1, open, rm - 1, tmp);
		}
	}
	else
	{
		tmp[i] = s[i];
		solve(s, i + 1, open, rm, tmp);
	}
}

int main (int ac, char **av)
{
	if (ac != 2)
		return (1);

	char *s = av[1];
	int i = 0, open = 0, close = 0;
	while (s[i])
	{
		if (s[i] == '(')
			open++;
		else if (s[i] == ')')
		{
			if (open > 0)
				open--;
			else
				close++;
		}
		i++;
	}
	char tmp[i + 1];
	solve(s, 0, 0, open + close, tmp);
	return (0);
}
