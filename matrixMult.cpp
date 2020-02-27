#include "matrixMult.h"

bool isEqual(double **A, double **B, int m, int n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if(A[i][j]!=B[i][j])
				return false;
		}
	}
	return false;
}

bool isEqual(double *A, double *B, int m, int n)
{
	int x = m*n;
	for(int i=0;i<x;++i)
	{
		if(A[i]!=B[i])
			return false;
	}
	return true;
}

void matrixMultiply(double **A, double **B, double **C, int m, int n, int p)
{
	for(int i=0;i<m;++i)
	{
		for(int j=0;j<p;++j)
		{
			double sum = 0;
			for(int k=0;k<n;++k)
				sum += A[i][k] * B[k][j];
			C[i][j] = sum;
		}
	}
}

void matrixMultiply(double *A, double *B, double *C, int m, int n, int p)
{
	for(int i=0;i<m;++i)
	{
		for(int j=0;j<p;++j)
		{
			double sum = 0;
			for(int k=0;k<n;++k)
				sum += A[i*n + k] * B[k*p + j];
			C[i*p + j] = sum;
		}
	}
}