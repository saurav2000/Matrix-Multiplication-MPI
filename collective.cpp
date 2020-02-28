#include <mpi.h>
#include "matrixMult.h"

using namespace std;

int main(int argc, char *argv[])
{
	Init(&argc, &argv);
	int n = stoi(argv[1]);;
	double *A = (double*)malloc(32 * n * sizeof(double));
	double *B = (double*)malloc(32 * n * sizeof(double));
	double *C = (double*)malloc(n * n * sizeof(double));
	double *C_ser = (double*)malloc(n * n * sizeof(double));	

	int rank, size;
	Status status;
	MPI_Comm_rank(WORLD, &rank);
	MPI_Comm_size(WORLD, &size);

	if(rank==0)
	{
		// Initialising A and B
		srand(time(NULL));
		for(int i=0;i<n;++i)
		{
			for(int j=0;j<32;++j)
			{
				A[i*32 + j] = drand48();
				B[j*n + i] = drand48();
			}
		}
	}

	auto start = std::chrono::high_resolution_clock::now();

	// Broadcasting B to all
	Broadcast(B, n*32, DOUBLE, 0, WORLD);

	// Using Scatterv to scatter unevenly
	int chunk_size = n/size, offset = n%size;
	int count_arr[size] = {(chunk_size*32)};
	count_arr[0]+= (offset*32);
	int displs[size] = {0};
	for(int i=1;i<size;++i)
	{
		displs[i] = (offset + chunk_size*i)*32;
		count_arr[i] = chunk_size*32;
	}

	Scatterv(A, count_arr, displs, DOUBLE, A, count_arr[rank], DOUBLE, 0, WORLD);

	// Multiplying data from scatterv
	matrixMultiply(A, B, C, count_arr[rank]/32, 32, n);

	// Changing count and displacements for Gather
	for(int i=0;i<size;++i)
	{
		count_arr[i] = (count_arr[i]/32)*n;
		displs[i] = (displs[i]/32)*n;
	}

	// Gathering all c
	Gatherv(C, count_arr[rank], DOUBLE, C, count_arr, displs, DOUBLE, 0, WORLD);
	auto end = std::chrono::high_resolution_clock::now();

	if(rank==0)
	{
		// Serial Multiplication
		matrixMultiply(A, B, C_ser, n, 32, n);
		cout<<isEqual(C, C_ser, n, n)<<"\n";

		// Printing Time Taken
		cout<<std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count()<<"\n";
	}
	
	Finalize();
	return 0;
}