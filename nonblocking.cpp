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

		auto start = std::chrono::high_resolution_clock::now();

		Request request_send, request_recv[size];
		// Sending B to all cores
		for(int i=1;i<size;++i)
			Isend(B, n*32, DOUBLE, i, 0, WORLD, &request_send);

		// Sending A in chunks and then receving chunks of C
		int chunk_size = n/size, offset = n%size;

		for(int i=1;i<size;++i)
		{
			Isend(&chunk_size, 1, INT, i, 0, WORLD, &request_send);
			Isend(A+((offset+i*chunk_size)*32), chunk_size*32, DOUBLE, i, 0, WORLD, &request_send);
		}
		
		// Calculating the first part
		matrixMultiply(A, B, C, offset+chunk_size, 32, n);
		// Receiving other parts
		for(int i=1;i<size;++i)
			Irecv(C+((offset+i*chunk_size)*n), chunk_size*n, DOUBLE, i, 0, WORLD, &request_recv[i]);

		for(int i=1;i<size;++i)
			Wait(&request_recv[i], &status);

		auto end = std::chrono::high_resolution_clock::now();
		cout<<std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count()<<"\n";
	}
	else
	{
		Request req1, req2, req3;
		Irecv(B, n*32, DOUBLE, 0, 0, WORLD, &req1);
		int chunk_size;
		Irecv(&chunk_size, 1, INT, 0, 0, WORLD, &req2);
		Wait(&req2, &status);
		Irecv(A, chunk_size*32, DOUBLE, 0, 0, WORLD, &req3);
		Wait(&req1, &status);
		Wait(&req3, &status);
		matrixMultiply(A, B, C, chunk_size, 32, n);
		Isend(C, chunk_size*n, DOUBLE, 0, 0, WORLD, &req1);
	}

	Finalize();
	return 0;
}