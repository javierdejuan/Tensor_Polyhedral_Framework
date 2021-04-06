#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define ERROR   1.e-20
#define MAX_VAL 5

#define M 256
#define N 256
#define P 256

void example_reference(double A[M][P], double B[M][P],
                       double C[M][N], double D[P][M][N]) {

  size_t i, j, k;

  for(i=0;i<M;i++)
    for(k=0;k<P;k++)
      B[i][k] = A[i][k];

  for(i=0;i<M;i++)
    for(j=0;k<N;j++)
      for(k=0;k<P;k++)
        C[i][j] += B[i][k]+D[k][i][j];
}

void example_kernel(double A[M][P], double B[M][P],
                       double C[M][N], double D[P][M][N]) {

  size_t i, j, k;
  #pragma scop
  for(i=0;i<M;i++)
    for(k=0;k<P;k++)
      B[i][k] = A[i][k];

  for(i=0;i<M;i++)
    for(j=0;k<N;j++)
      for(k=0;k<P;k++)
        C[i][j] += B[i][k]+D[k][i][j];
  #pragma endscop
}

int main() {

  double* AR = malloc( M * P * sizeof(double));
  double* AK = malloc( M * P * sizeof(double));
  double* BR = malloc( M * P * sizeof(double));
  double* BK = malloc( M * P * sizeof(double));
  double* CR = malloc( M * N * sizeof(double));
  double* CK = malloc( M * N * sizeof(double));
  double* DR = malloc( P * M * N * sizeof(double));
  double* DK = malloc( P * M * N * sizeof(double));
  clock_t start, end;

  // initializing by random values
  srand((unsigned int)time(NULL));
  for(size_t i = 0 ; i < M * P ; i++){
    AR[i] = AK[i] = (double)rand()/(double)(RAND_MAX/MAX_VAL);
    BR[i] = BK[i] = (double)rand()/(double)(RAND_MAX/MAX_VAL);
  }
  for(size_t i = 0 ; i < M * N ; i++){
    CR[i] = CK[i] = (double)rand()/(double)(RAND_MAX/MAX_VAL);
  }
  for(size_t i = 0 ; i < P * M * N ; i++){
    DR[i] = DK[i] = (double)rand()/(double)(RAND_MAX/MAX_VAL);
  }
 
  start = clock();
  example_reference((double (*)[P])AR, (double (*)[P]) BR,
                    (double (*)[N])CR, (double (*)[M][N]) DR);
  end   = clock();
  printf("Reference time: %3.5lf s\n", ((double)(end - start)) / CLOCKS_PER_SEC);
 
  start = clock();
  example_kernel(   (double (*)[P])AR, (double (*)[P]) BR,
                    (double (*)[N])CR, (double (*)[M][N]) DR);
  end   = clock();
  printf("Kernel time: %3.5lf s\n", ((double)(end - start)) / CLOCKS_PER_SEC);

  // check if the results differs from reference
  for( size_t i = 0 ; i < M * P; i++){
    if(fabs(CR[i] - CK[i]) > ERROR){
      printf("Bad results!\n");
      exit(1);
    }
  }
  printf("Process finished correctly\n");
}
