#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define INT_MAX 2147483647

/*
compile: gcc -O0 -fopenmp mergeSort_2.c -o mergeSort_2
usage: ./mergeSort_1.5 ARRAY_LEN THREAD_NUM
set THREAD_NUM=0 for serial without openMP. Otherwise parallel
*/

void merge(int* array, int start, int mid, int end, int* temp){

    int lenLeft = mid - start + 1 + 1;
    int lenRight = end - mid  + 1;
    for(int i=start; i<=end; i++){
        temp[i]=array[i];
    }


    int currLeft=start;
    int currRight=mid+1;
    int currIndex=start;
    while(currLeft<=mid || currRight<=end){
        if(currLeft==mid+1) array[currIndex]=temp[currRight++];
        else if(currRight==end+1) array[currIndex]=temp[currLeft++];
        else array[currIndex]= temp[currLeft]<=temp[currRight]? temp[currLeft++]: temp[currRight++];
        currIndex++;
    }

    // printf("array after merge:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    //     if(i==mid) printf("|");
    // }
    // printf("\n");
}

int testSorted(int* array, int len){
    for(int i=0; i<len-1; i++){
        if(array[i]>array[i+1]) return 0;
    }
    return 1;
}

void mergeSort_recursive(int* array, int start, int end, int* temp){
    if(start>=end) return;
    int mid=start+(end-start)/2;
    mergeSort_recursive(array, start, mid, temp);
    mergeSort_recursive(array, mid+1, end, temp);
    merge(array, start, mid, end, temp);
}

void mergeSort_parallel_recursive(int* array, int start, int end, int* temp){
    if(start>=end) return;
    int mid=start+(end-start)/2;
    //printf("1");
    #pragma omp task
    mergeSort_parallel_recursive(array, start, mid, temp);
    #pragma omp task
    mergeSort_parallel_recursive(array, mid+1, end, temp);
    #pragma omp taskwait
    merge(array, start, mid, end, temp);
}

void mergeSort_parallel(int* array, int start, int end, int threadNum, int* temp){
    omp_set_num_threads(threadNum);
    #pragma omp parallel
    #pragma omp single
    printf("threadNum=%d\n",omp_get_num_threads());
    mergeSort_parallel_recursive(array, start, end, temp);
}


int main(int argc, char* argv[]) {
    if(argc!=3){
        printf("./mergeSort ARRAY_LENGTH THREAD_NUM\n");
        return 0;
    }
    
    
    int threadNum = atoi(argv[2]);
    int arrayLen = atoi(argv[1]);
    int iteration = arrayLen<=(1<<15)?100:10;
    int* array=(int*)malloc(arrayLen*sizeof(int));
    int* temp=(int*)malloc(arrayLen*sizeof(int));

    
    // printf("random array:\n");
    // for(int i=0; i < arrayLen; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
    
    double timeTotal=0;
    for(int currIter=0; currIter<iteration; currIter++){
        srand(time(0));
        for(int i=0; i < arrayLen; i++) {
            array[i]=(rand()%1000);
        }
        double t1= omp_get_wtime();
        if(threadNum==0)
            mergeSort_recursive(array, 0, arrayLen-1, temp);
        else
            mergeSort_parallel(array, 0, arrayLen-1, threadNum, temp);
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