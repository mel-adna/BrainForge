#include <float.h>
#include <math.h>
#include <stdio.h>

#define MAX_CITIES 11

float	distance(float a[2], float b[2])
{
	return (sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1])));
}

float	calculate_path_length(float cities[][2], int *perm, int n)
{
	float	len;

	len = 0.0f;
	for (int i = 0; i < n - 1; i++)
		len += distance(cities[perm[i]], cities[perm[i + 1]]);
	len += distance(cities[perm[n - 1]], cities[perm[0]]);
	return (len);
}

void	swap(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	permute(int *perm, int start, int n, float cities[][2], float *min_len)
{
	float	len;

	if (start == n)
	{
		len = calculate_path_length(cities, perm, n);
		if (len < *min_len)
			*min_len = len;
		return ;
	}
	for (int i = start; i < n; i++)
	{
		swap(&perm[start], &perm[i]);
		permute(perm, start + 1, n, cities, min_len);
		swap(&perm[start], &perm[i]);
	}
}

int	main(void)
{
	float	cities[MAX_CITIES][2];
	int		perm[MAX_CITIES];
	int		n;
	float	min_len;

	n = 0;
	while (scanf("%f, %f", &cities[n][0], &cities[n][1]) == 2 && n < MAX_CITIES)
	{
		perm[n] = n;
		n++;
	}
	min_len = FLT_MAX;
	permute(perm, 0, n, cities, &min_len);
	printf("%.2f\n", min_len);
	return (0);
}
