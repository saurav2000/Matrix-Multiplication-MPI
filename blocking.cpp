#include <mpi.h>
#include "matrixMult.h"

using namespace std;

void singlePointerChunks(int argc, char *argv[])
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
		
		auto start = std::chrono::high_resolution_clock::now();
		// Sending B to all cores
		for(int i=1;i<size;++i)
			Send(B, n*32, DOUBLE, i, 0, WORLD);

		// Sending A in chunks and then receving chunks of C
		int chunk_size = n/size, offset = n%size;

		for(int i=1;i<size;++i)
		{
			Send(&chunk_size, 1, INT, i, 0, WORLD);
			Send(A+((offset+i*chunk_size)*32), chunk_size*32, DOUBLE, i, 0, WORLD);
		}
		
		// Calculating the first part
		matrixMultiply(A, B, C, offset+chunk_size, 32, n);
		// Receiving other parts
		for(int i=1;i<size;++i)
			Recv(C+((offset+i*chunk_size)*n), chunk_size*n, DOUBLE, i, 0, WORLD, &status);
		
		auto end = std::chrono::high_resolution_clock::now();

		// Printing Time Taken
		cout<<std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count()<<"\n";
		// writeToFile(C, n, n, "alskdf.txt");

		// Serial Multiplication		
		matrixMultiply(A, B, C_ser, n, 32, n);
		cout<<isEqual(C, C_ser, n, n)<<"\n";
	}
	else
	{
		Recv(B, n*32, DOUBLE, 0, 0, WORLD, &status);
		int chunk_size;
		Recv(&chunk_size, 1, INT, 0, 0, WORLD, &status);
		Recv(A, chunk_size*32, DOUBLE, 0, 0, WORLD, &status);
		matrixMultiply(A, B, C, chunk_size, 32, n);
		Send(C, chunk_size*n, DOUBLE, 0, 0, WORLD);
	}

	Finalize();
}

void doublePointerChunks(int argc, char *argv[])
{
	Init(&argc, &argv);
	int n = stoi(argv[1]);;
	double **A = (double**)malloc(n * sizeof(double*));
	double **B = (double**)malloc(32 * sizeof(double*));
	double **C = (double**)malloc(n * sizeof(double*));
	for(int i=0;i<n;++i)
	{
		A[i] = (double*)malloc(32 * sizeof(double));
		C[i] = (double*)malloc(n * sizeof(double));
	}
	for(int i=0;i<32;++i)
		B[i] = (double*)malloc(n * sizeof(double));

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
				A[i][j] = drand48();
				B[j][i] = drand48();
			}
		}
		
		auto start = std::chrono::high_resolution_clock::now();
		// Sending B to all cores

		for(int i=1;i<size;++i)
		{
			for(int j=0;j<32;++j)
				Send(B[j], n, DOUBLE, i, 0, WORLD);
		}

		// Sending A in chunks and then receving chunks of C
		int chunk_size = n/size, offset = n%size;

		for(int i=1;i<size;++i)
		{
			Send(&chunk_size, 1, INT, i, 0, WORLD);
			for(int j=offset+i*chunk_size;j<offset+(i+1)*chunk_size; ++j)
				Send(A[j], 32, DOUBLE, i, 0, WORLD);
		}
		
		// Calculating the first part
		matrixMultiply(A, B, C, offset+chunk_size, 32, n);
		// Receiving other parts
		for(int i=1;i<size;++i)
		{
			for(int j=offset+i*chunk_size;j<offset+(i+1)*chunk_size; ++j)
				Recv(C[j], n, DOUBLE, i, 0, WORLD, &status);
		}
		
		auto end = std::chrono::high_resolution_clock::now();
		cout<<std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count()<<"\n";
		// writeToFile(C, n, n, "alskdf.txt");
	}
	else
	{
		for(int i=0;i<32;++i)
			Recv(B[i], n, DOUBLE, 0, 0, WORLD, &status);
		int chunk_size;
		Recv(&chunk_size, 1, INT, 0, 0, WORLD, &status);

		for(int i=0;i<chunk_size;++i)
			Recv(A[i], 32, DOUBLE, 0, 0, WORLD, &status);
		matrixMultiply(A, B, C, chunk_size, 32, n);

		for(int i=0;i<chunk_size;++i)
			Send(C[i], n, DOUBLE, 0, 0, WORLD);
	}

	Finalize();
}

void singlePointerRows(int argc, char *argv[])
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
		// Sending B to all cores
		for(int i=1;i<size;++i)
			Send(B, n*32, DOUBLE, i, 0, WORLD);

		// Sending A in chunks and then receving chunks of C
		int chunk_size = n/size, offset = n%size;

		for(int i=1;i<size;++i)
		{
			Send(&chunk_size, 1, INT, i, 0, WORLD);
			Send(A+((offset+i*chunk_size)*32), chunk_size*32, DOUBLE, i, 0, WORLD);
		}
		
		// Calculating the first part
		matrixMultiply(A, B, C, offset+chunk_size, 32, n);
		// Receiving other parts
		for(int i=1;i<size;++i)
			Recv(C+((offset+i*chunk_size)*n), chunk_size*n, DOUBLE, i, 0, WORLD, &status);
		
		auto end = std::chrono::high_resolution_clock::now();
		cout<<std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count()<<"\n";
	}
	else
	{
		Recv(B, n*32, DOUBLE, 0, 0, WORLD, &status);
		int chunk_size;
		Recv(&chunk_size, 1, INT, 0, 0, WORLD, &status);
		Recv(A, chunk_size*32, DOUBLE, 0, 0, WORLD, &status);
		matrixMultiply(A, B, C, chunk_size, 32, n);
		Send(C, chunk_size*n, DOUBLE, 0, 0, WORLD);
	}

	Finalize();
}

int main(int argc, char *argv[])
{
	singlePointerChunks(argc, argv);
	return 0;
}