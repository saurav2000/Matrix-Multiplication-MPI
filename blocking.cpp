#include <cstdio>
#include <iostream>
#include <chrono>
#include <fstream>
#include <mpi.h>
#include <ctime>
#include "matrixMult.h"

using namespace std;

int main(int argc, char const *argv[])
{
	Init(&argc, &argv);
	int n = stoi(argv[1]);
	double *A = (double*)malloc(32 * n * sizeof(double));
	double *B = (double*)malloc(32 * n * sizeof(double));
	double *C = (double*)malloc(n * n * sizeof(double));
	double *C_serial = (double*)malloc(n * n * sizeof(double));

	int rank, status, size;
	MPI_Comm_rank(WORLD, &rank);
	MPI_Comm_size(WORLD, &size);

	if(rank==0)
	{
		// Initialising A and B
		srand(time(NULL));
		for(int i=0;i<m;++i)
		{
			for(int j=0;j<32;++j)
			{
				A[i*32 + j] = drand48();
				B[j*n + i] = drand48();
			}
		}
		
		// Sending B to all cores
		for(int i=1;i<size;++i)
			Send(B, n*32, Double, i, MPI_ANY_TAG, WORLD);

		// Sending A in chunks and then receving chunks of C
		int chunk_size = n/size, offset = n%size;
		for(int i=1;i<size;++i)
			Send(A+offset+i*chunk_size, chunk_size, Double, i, MPI_ANY_TAG, WORLD);
		
	}
	else
	{

	}
	


	auto start = std::chrono::high_resolution_clock::now();
	matrixMultiply(A, B, C, n, 32, n);
	auto end = std::chrono::high_resolution_clock::now();
	double time = std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count();
	cout<<time<<"\n";

	// Finalize();
	return 0;
}