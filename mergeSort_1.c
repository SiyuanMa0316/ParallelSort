#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define INT_MAX 2147483647

/*
compile: gcc -O0 -fopenmp mergeSort_1.c -o mergeSort_1
usage: ./mergeSort_1 ARRAY_LEN THREAD_NUM
set THREAD_NUM=0 for serial without openMP. Otherwise parallel
*/

void merge(int* array, int start, int mid, int end){

    // printf("array before merge:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    //     if(i==mid) printf("|");
    // }
    // printf("\n");

    int lenLeft = mid - start + 1 + 1;
    int lenRight = end - mid  + 1;
    int* tempLeft=(int*)malloc(lenLeft*sizeof(int));
    int* tempRight=(int*)malloc(lenRight*sizeof(int));
    for(int i=0; i<lenLeft-1; i++){
        tempLeft[i]=array[start+i];
    }
    tempLeft[lenLeft-1]=INT_MAX;
    for(int i=0; i<lenRight-1; i++){
        tempRight[i]=array[mid+1+i];
    }
    tempRight[lenRight-1]=INT_MAX;
    // printf("tempLeft:\n");
    // for(int i=0; i < lenLeft; i++) {
    //     printf("%d ", tempLeft[i]);
    // }
    // printf("\n");
    // printf("tempRight:\n");
    // for(int i=0; i < lenRight; i++) {
    //     printf("%d ", tempRight[i]);
    // }
    // printf("\n");

    int currLeft=0;
    int currRight=0;
    int currIndex=start;
    while(currLeft<lenLeft-1 || currRight<lenRight-1){
        array[currIndex]= tempLeft[currLeft]<=tempRight[currRight]? tempLeft[currLeft++]: tempRight[currRight++];
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

void mergeSort_recursive(int* array, int start, int end){
    if(start>=end) return;
    int mid=start+(end-start)/2;
    mergeSort_recursive(array, start, mid);
    mergeSort_recursive(array, mid+1, end);
    merge(array, start, mid, end);
}

void mergeSort_parallel_recursive(int* array, int start, int end){
    if(start>=end) return;
    int mid=start+(end-start)/2;
    #pragma omp task
    mergeSort_parallel_recursive(array, start, mid);
    #pragma omp task
    mergeSort_parallel_recursive(array, mid+1, end);
    #pragma omp taskwait
    merge(array, start, mid, end);
}

void mergeSort_parallel(int* array, int start, int end, int threadNum){
    omp_set_num_threads(threadNum);
    #pragma omp parallel
    #pragma omp single
    printf("threadNum=%d\n",omp_get_num_threads());
    mergeSort_parallel_recursive(array, start, end);
}


int main(int argc, char* argv[]) {
    if(argc!=3){
        printf("./mergeSort ARRAY_LENGTH THREAD_NUM\n");
        return 0;
    }
    
    int iteration = 10;
    int threadNum = atoi(argv[2]);
    int arrayLen = atoi(argv[1]);
    int* array=(int*)malloc(arrayLen*iteration*sizeof(int));

    srand(time(0));
    for(int i=0; i < arrayLen*iteration; i++) {
        array[i]=(rand()%1000);
    }
    // printf("random array:\n");
    // for(int i=0; i < arrayLen; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
    double t1= omp_get_wtime();
    for(int currIter=0; currIter<iteration; currIter++){
        if(threadNum==0)
            mergeSort_recursive(array, currIter*arrayLen, currIter*arrayLen + arrayLen-1);
        else
            mergeSort_parallel(array, currIter*arrayLen, currIter*arrayLen + arrayLen-1, threadNum);
    }

    double t2= omp_get_wtime();

    // printf("sorted array:\n");
    // for(int i=0; i < arrayLen; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
    for(int currIter=0; currIter<iteration; currIter++){
        if(testSorted(array + currIter*arrayLen, arrayLen)) printf("test passed!\n");
        else printf("test NOT passed!\n");
    }

    printf("time=%f\n", (t2-t1)/iteration);


    return 0;
}