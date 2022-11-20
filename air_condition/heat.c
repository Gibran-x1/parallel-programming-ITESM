#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define Mc 7 
#define Mr 6 

//Declaracion de variables y matrices utilizadas asi como definir los threads
int i,j;
pthread_mutex_t stop;
bool matrixb = 0;
int TempMatrix[Mr][Mc];
//Matriz de temperatura inicial
char Matrix_data[Mr][Mc] = {{'C','C','C','C','C','C','C'},
                            {'C','C','C','C','C','C','C'},
                            {'C','C','H','H','H','C','C'},
                            {'C','C','H','H','H','C','C'},
                            {'C','C','H','H','H','C','C'},
                            {'C','C','C','C','C','C','C'}};

void completeMatrix(){
  for(int a = 0; a < Mr; a++){
    for(int b = 0; b < Mc; b++){
      if (TempMatrix[a][b] == -1){
        return;
      }
    }
  }
  matrixb = 1;
}


//Recorrer la matriz hasta generar los threads
void matrixThreads (int* Mrow, int* Mcol){
    int rowleft = -1;
    int rowrigth = -1;
    int colup = -1;
    int coldown = -1;
    if(*Mrow == 0){
        rowleft = 0;
        rowrigth = 0;
    }else if(*Mrow == Mr - 1){
        rowrigth = 0;
    }else if(*Mcol == 0){
        colup = 0;
        coldown = 0;
    }else if(*Mcol == Mc -1){
        coldown = 0;
    }
    int temp = rowleft + rand() %(2-rowrigth);
    int temp2 = colup + rand() %(2-coldown);
    *Mrow += temp;
    *Mcol += temp2;
}

void* asignar_temperatura(void* time){
    int row = rand() % Mr;
    int col = rand() % Mc;
    while(true){
        if(time == 0){
            completeMatrix();
        }
        if(matrixb){
            pthread_exit(NULL);
        }
        matrixThreads(&row, &col);
        pthread_mutex_lock(&stop);
        if(Matrix_data[row][col] == 'C'){
            TempMatrix[row][col] = rand() % 61;
        }else{
            TempMatrix[row][col] = 61 + rand() %(100-61);
        }
        pthread_mutex_unlock(&stop);
    }    
}

void printMatrixA(int row, int col, char matrix[row][col]){
    printf("Matrix Data: \n");
    for(i = 0; i < row;i++){
        for(j = 0; j < col; j++){
            printf("%c ",Matrix_data[i][j]);
            TempMatrix[i][j] =-1;
        }
        printf("\n");
      }
}

void printMatrixB(int row, int col, int matrix[row][col]){
    printf("Matrix temperature: \n");
    for(i = 0; i < row;i++){
        for(j = 0; j < col; j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]){

    printMatrixA(Mr,Mc,Matrix_data[Mr][Mc]);

  int num_threads = 4;
  if (argc > 1){
    num_threads = atoi(argv[1]);
  }
  pthread_t threads[num_threads];

  //Create threads
  int rc;
  srand(time(NULL));
  for(long k = 0; k < num_threads; k++){
      rc = pthread_create(&threads[k], NULL, asignar_temperatura, (void*) k);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }

  printMatrixB(Mr,Mc,TempMatrix);

  //Solucion ecuacion de calor
    int time = 100;
    int rubi = 2;
    int zafiro = 1;
    float esmeralda = (pow(zafiro,2))/(4*rubi);
    float omega = (rubi * esmeralda) / (zafiro * zafiro);
    int alpha[time][Mr][Mc];
    int x,y,z;
    #pragma omp parallel for private(x, y, z) shared (alpha, TempMatrix)
      for(int x = 0; x < time; x++){
        for(int y = 0; y < Mr; y++){
            for(int z = 0; z < Mc; z++){
                alpha[x][y][z] = TempMatrix[y][z];
            }
        }
    }

    #pragma omp parallel for private(x,y,z) shared (alpha)
      for(int x = 0; x < time - 1; x++){
        for(int y = 0; y < Mr ; y+= zafiro){
          for(int z = 0; z < Mc; z+= zafiro){
            alpha[x + 1][y][z] = omega * (alpha[x][y+1][z] + alpha[x][y-1][z] + alpha[x][y][z+1] + alpha[x][y][z-1] - 4*alpha[x][y][z]) + alpha[x][y][z];
            }
        }
    }
    printf("\nDear manager:");
    printf("\nThis is the heat equation for the factory:");
    printf("\nQ = %d * (%.3f / (%d)^2)\n", rubi, esmeralda, zafiro);
    pthread_mutex_destroy(&stop);
    return 0;
}
