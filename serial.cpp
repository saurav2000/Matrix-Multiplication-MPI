#include "matrixMult.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int n = stoi(argv[1]);

	double *A = (double*)malloc(32 * n * sizeof(double));
	double *B = (double*)malloc(32 * n * sizeof(double));
	double *C = (double*)malloc(n * n * sizeof(double));

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

	// Matrix Multiplication
	auto start = std::chrono::high_resolution_clock::now();
	matrixMultiply(A, B, C, n, 32, n);
	auto end = std::chrono::high_resolution_clock::now();

	// Printing Time Taken
	cout<<n<<"\n";
	cout<<std::chrono::duration_cast<std::chrono::duration<double> >(end - start).count()<<"\n";
	return 0;
}