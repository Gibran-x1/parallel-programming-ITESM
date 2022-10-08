#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define THREADS 8

int npuntos = 100000;
int circle_count = 0;
double radio = 1;
pthread_mutex_t lock;
int pointsThread;

unsigned int seed;

double npoints(){
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

int PuntosCirculo(double x, double y){
    double d = pow(radio,2) - (pow(x,2) + pow(y,2));

    if(d>=0){
        return 1;
    }else{
        return 0;
    }
}

void * ContadorPuntos(void * line){
    int cont = 0;
    pthread_mutex_lock(&lock);
    for (int j = 1; j <= pointsThread; j++){
     
        double xcoordinate = npoints();
        double ycoordinate = npoints();
        int circ = PuntosCirculo(xcoordinate,ycoordinate);
        
        if (circ==1) {
            circle_count = circle_count + 1;
        }else{
            cont++;
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argx, char *argy[]){
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
            return 1;
    }
    
    seed = time(NULL);
    pthread_t threads[THREADS];
    pointsThread= npoints/THREADS;


    for(int i=0; i < THREADS; i++){
        pthread_create(&threads[i], NULL, ContadorPuntos, NULL);
    }
    for(int i=0; i < THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    double PI = 4.0*(double)circle_count/(double)npuntosts;
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("PI = %f\nTime used=%f",PI, cpu_time_used);
    pthread_mutex_destroy(&lock);
}
