#include <stdio.h>

void Matrix_Multiply(float *A, float *B, float *C, int m, int n, int p)
{
	int i, j, k;
	for (i = 0; i < m; i++i)
	{
		for (j = 0; j < p; j++)
		{
			C[i*p + j] = 0;
			for (k = 0; k < n; k++)
				C[i*p + j] += A[i*n + k] * B[k*p + j];
		}
	}
}

int isEqual(float *A, float *B, int m, int n)
{
	int x = m*n;
	for(int i=0;i<x;++i)
	{
		if(A[i]!=B[i])
			return 0;
	}
	return 1;
}

int main(int argc, char const *argv[])
{
	
	return 0;
}