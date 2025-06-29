#include <stdlib.h>
#include <stdio.h>

void	solve(int *board, int n, int row)
{
	if (row == n)
		for (int i = 0; i < n; i++)
			printf("%d%c", board[i], i < n - 1 ? ' ': '\n');
	for (int column = 0; column < n; column++)
	{
		int ok = 1;
		for (int i = 0; i < row; i++)
			if (board[i] == column || board[i] - i == column - row || board[i] + i == column + row)
				ok = 0;
		if (ok == 1)
		{
			board[row] = column;
			solve(board, n, row + 1);
		}
	}
}

int main(int ac, char **av)
{
	int n;
	int *board;
	
	if (ac == 2)
	{
		n = atoi(av[1]);
		if (n <= 0)
			return (0);
		board = malloc(sizeof(int) * n);
		if (!board)
			return (1);
		solve(board, n, 0);
	}
	return (0);
}
