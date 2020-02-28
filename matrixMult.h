#include <iostream>
#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>

#define WORLD MPI_COMM_WORLD
#define Init MPI_Init
#define Finalize MPI_Finalize
#define Send MPI_Send
#define Recv MPI_Recv
#define INT MPI_INT
#define DOUBLE MPI_DOUBLE
#define Status MPI_Status
#define Broadcast MPI_Bcast
#define Gatherv MPI_Gatherv
#define Scatterv MPI_Scatterv
#define Request MPI_Request
#define Isend MPI_Isend
#define Irecv MPI_Irecv
#define Wait MPI_Wait

bool isEqual(double **A, double **B, int m, int n);
bool isEqual(double *A, double *B, int m, int n);
void matrixMultiply(double **A, double **B, double **C, int m, int n, int p);
void matrixMultiply(double *A, double *B,  double *C, int m, int n, int p);
void writeToFile(double **mat, int m, int n, char *file);
void writeToFile(double *mat, int m, int n, char *file);