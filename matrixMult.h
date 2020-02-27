#define WORLD MPI_COMM_WORLD
#define Finalize MPI_Finalize
#define Send MPI_Send
#define Recv MPI_Recv
#define Init MPI_Init
#define Int MPI_INT
#define Double MPI_DOUBLE
#define Status MPI_Status
// #define 

bool isEqual(double **A, double **B, int m, int n);
bool isEqual(double *A, double *B, int m, int n);
void matrixMultiply(double **A, double **B, double **C, int m, int n, int p);
void matrixMultiply(double *A, double *B,  double *C, int m, int n, int p);
void printToFile(double **mat, int m, int n, char *file);
void printToFile(double *mat, int m, int n, char *file);