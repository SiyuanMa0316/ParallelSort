#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<omp.h>
#define INT_MAX 2147483647
#define INT_MIN -2147483647-1

/*
compile: gcc -O0 -fopenmp qsort.c -o qsort
usage: ./qsort ARRAY_LEN 0
*/


int testSorted(int* array, int len){
    for(int i=0; i<len-1; i++){
        if(array[i]>array[i+1]) return 0;
    }
    return 1;
}

int compare_function(const void *a,const void *b) {
    int f = *((int*)a);
    int s = *((int*)b);
    if (f > s) return 1;
    if (f < s) return -1;
    return 0;
}

void mySort(int* array, int start, int end){
    
    qsort(array, end-start+1, sizeof(int), compare_function);

    // printf("sorted array:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
}

int main(int argc, char* argv[]) {
    if(argc!=3){
        printf("./sampleSort ARRAY_LENGTH THREAD_NUM\n");
        return 0;
    }
    
    
    int threadNum = atoi(argv[2]);
    int arrayLen = atoi(argv[1]);
    int iteration = arrayLen<=(1<<15)?100:10;
    //int iteration = 1;
    int* array=(int*)malloc(arrayLen*sizeof(int));

    
    
    
    double timeTotal=0;
    for(int currIter=0; currIter<iteration; currIter++){
        srand(time(0));
        for(int i=0; i < arrayLen; i++) {
            array[i]=(rand()%1000);
        }
        // for(int i=0; i < arrayLen; i++) {
        //     array[i]=arrayLen-i;
        // }

        // printf("random array:\n");
        // for(int i=0; i < arrayLen; i++) {
        //     printf("%d ", array[i]);
        // }
        // printf("\n");

        double t1= omp_get_wtime();
        if(threadNum==0)
            mySort(array, 0, arrayLen-1);
        else
            mySort(array, 0, arrayLen-1);
        double t2= omp_get_wtime();
        timeTotal=timeTotal+t2-t1;
        
    }

    

    // printf("sorted array:\n");
    // for(int i=0; i < arrayLen; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
    int passed=1;
    for(int currIter=0; currIter<iteration; currIter++){
        if(testSorted(array, arrayLen)==0) passed=0;
    }
    if(passed) printf("test passed!\n");
    else printf("test NOT passed!\n");

    printf("time = %f\n", timeTotal/iteration);


    return 0;
}