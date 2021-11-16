#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define INT_MAX 2147483647

/*
compile: gcc -O0 -fopenmp oddEvenSort.c -o oddEvenSort
usage: ./oddEvenSort ARRAY_LEN THREAD_NUM
set THREAD_NUM=0 for serial without openMP. Otherwise parallel
*/


int testSorted(int* array, int len){
    for(int i=0; i<len-1; i++){
        if(array[i]>array[i+1]) return 0;
    }
    return 1;
}

void oddEvenSort(int* array, int start, int end){
    int length=end-start+1;
    for(int phase=0; phase<length; phase++){
        
        if(phase%2==0){
            //#pragma omp parallel for 
            for(int i=start; i<end; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
        else{
            //#pragma omp parallel for 
            for(int i=start+1; i<end; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
        // for(int i=start; i <= end; i++) {
        //     printf("%d ", array[i]);
        // }
        // printf("\n");
    }
}

void oddEvenSort_parallel(int* array, int start, int end, int threadNum){
    omp_set_num_threads(threadNum);
    int length=end-start+1;
    for(int phase=0; phase<length; phase++){
        
        if(phase%2==0){
            #pragma omp parallel for 
            for(int i=start; i<end; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
        else{
            #pragma omp parallel for 
            for(int i=start+1; i<end; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
    }
}


int main(int argc, char* argv[]) {
    if(argc!=3){
        printf("./oddEvenSort ARRAY_LENGTH THREAD_NUM\n");
        return 0;
    }
    
    
    int threadNum = atoi(argv[2]);
    int arrayLen = atoi(argv[1]);
    int iteration = arrayLen<=(1<<12)?10:5;
    //int iteration = 1;
    int* array=(int*)malloc(arrayLen*sizeof(int));
    int* temp=(int*)malloc(arrayLen*sizeof(int));

    
    
    
    double timeTotal=0;
    for(int currIter=0; currIter<iteration; currIter++){
        srand(time(0));
        for(int i=0; i < arrayLen; i++) {
            array[i]=(rand()%1000);
        }
        // printf("random array:\n");
        // for(int i=0; i < arrayLen; i++) {
        //     printf("%d ", array[i]);
        // }
        // printf("\n");
        double t1= omp_get_wtime();
        if(threadNum==0)
            oddEvenSort(array, 0, arrayLen-1);
        else
            oddEvenSort_parallel(array, 0, arrayLen-1, threadNum);
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