//Codigo de multiplicacion de matrices utilizando threads
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define Pc 100 //columnas de la matriz AB
#define Pr 100 //filas de la matriz AB

int main(int argc, char** argv){
  int threadcnt = 1;
  if(argc > 1){
    threadcnt = atoi(argv[1]);
  }
  omp_set_num_threads( threadcnt );
  printf("OMP defined, threadcnt = %d\n", threadcnt);
  int A[Pr][Pc] = {};
  int B[Pr][Pc] = {};
  for(int i = 0; i< Pr; i++){
    for(int j = 0; j < Pc;j++){
      A[i][j] = rand() % 10;
      B[i][j] = rand() % 10;
    }
  }
  int Nr = sizeof(A)/sizeof(A[0]);
  int Nc = sizeof(A[0]) / sizeof(A[0][0]);
  int Mr = sizeof(B)/sizeof(B[0]);
  int Mc = sizeof(B[0]) / sizeof(B[0][0]);
  int i;
  int j;
  int k;
  if(Nc != Mr){
    printf("Las matrices no se pueden multiplicar ");
  }else{
    int C[Pr][Pc];
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for(i = 0; i < Nr; i++){
      for(j = 0; j < Mc; j++){
        C[i][j] = 0;
        for(k = 0; k < Mr; k++){
          C[i][j] += (A[i][k] * B[k][j]);
        }
      }
    }
    int c_r = sizeof(C) / sizeof(C[0]);
    int c_c = sizeof(C[0]) / sizeof(C[0][0]);
    for(int i = 0; i < c_r; i++){
      for(int j = 0; j < c_c; j++){
        printf("%d\t",C[i][j]);
      }
      printf("\n");
    }
  }
  printf("Proceso Terminado");
  return 0;
}
