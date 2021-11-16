#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define INT_MAX 2147483647

/*
compile: gcc -O0 -fopenmp sampleSort.c -o sampleSort
usage: ./sampleSort ARRAY_LEN THREAD_NUM
set THREAD_NUM=0 for serial without openMP. Otherwise parallel
*/


int testSorted(int* array, int len){
    for(int i=0; i<len-1; i++){
        if(array[i]>array[i+1]) return 0;
    }
    return 1;
}
int partition(int* array, int start, int end){
    if(start>end){
        //printf("invalid!\n");
        return start;
    }
    else if(start==end) return end;
    // pivot (Element to be placed at right position)
    int pivotVal = array[end];  
 
    int i = start - 1;  // Index of smaller element and indicates the 
                   // right position of pivot found so far

    for (int j = start; j <= end- 1; j++)
    {
        // If current element is smaller than the pivot
        if (array[j] < pivotVal)
        {
            i++;    // increment index of smaller element
            int temp=array[i];
            array[i]=array[j];
            array[j]=temp;
        }
        // printf("processing array:\n");
        // for(int i=start; i<15; i++){
        //     printf("%d ", pivot[i]);
        // }
        // printf("\n");
    }
    //swap arr[i + 1] and arr[high]
    i++;
    array[end]=array[i];
    array[i]=pivotVal;
    return i;
}

void sampleSort(int* array, int start, int end){
    int pivot[15];
    pivot[7]=partition(array, start, end);
    printf("partitioned array: pivot at %d\n", pivot[7]);
    for(int i=start; i<=end; i++){
        printf("%d ", array[i]);
    }
    printf("\n");


    pivot[3]=partition(array, start, pivot[7]-1);
    printf("partitioned array: pivot at %d\n", pivot[3]);
    for(int i=start; i<=end; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    pivot[11]=partition(array, pivot[7]+1, end);
    printf("partitioned array: pivot at %d\n", pivot[11]);
    for(int i=start; i<=end; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    pivot[1]=partition(array, start, pivot[3]-1);
    pivot[5]=partition(array, pivot[3]+1, pivot[7]-1);
    pivot[9]=partition(array, pivot[7]+1, pivot[11]-1);
    pivot[13]=partition(array, pivot[11]+1, end);

    pivot[0]=partition(array, start, pivot[1]-1);
    pivot[2]=partition(array, pivot[1]+1, pivot[3]-1);
    pivot[4]=partition(array, pivot[3]+1, pivot[5]-1);
    pivot[6]=partition(array, pivot[5]+1, pivot[7]-1);
    pivot[8]=partition(array, pivot[7]+1, pivot[9]-1);
    pivot[10]=partition(array, pivot[9]+1, pivot[11]-1);
    pivot[12]=partition(array, pivot[11]+1, pivot[13]-1);
    pivot[14]=partition(array, pivot[13]+1, end);
    printf("pivot:\n");
    for(int i=0; i<15; i++){
        printf("%d ", pivot[i]);
    }
    printf("\n");
}

void sampleSort_parallel(int* array, int start, int end, int threadNum){
    int pivot[15];
    pivot[7]=partition(array, start, end);

    pivot[3]=partition(array, start, pivot[7]);
    pivot[11]=partition(array, pivot[7]+1, end);

    pivot[1]=partition(array, start, pivot[3]);
    pivot[5]=partition(array, pivot[3]+1, pivot[7]);
    pivot[9]=partition(array, pivot[7]+1, pivot[11]);
    pivot[13]=partition(array, pivot[11]+1, end);

    pivot[0]=partition(array, start, pivot[1]);
    pivot[2]=partition(array, pivot[1]+1, pivot[3]);
    pivot[4]=partition(array, pivot[3]+1, pivot[5]);
    pivot[6]=partition(array, pivot[5]+1, pivot[7]);
    pivot[8]=partition(array, pivot[7]+1, pivot[9]);
    pivot[10]=partition(array, pivot[9]+1, pivot[11]);
    pivot[12]=partition(array, pivot[11]+1, pivot[13]);
    pivot[14]=partition(array, pivot[13]+1, end);
}

int main(int argc, char* argv[]) {
    if(argc!=3){
        printf("./sampleSort ARRAY_LENGTH THREAD_NUM\n");
        return 0;
    }
    
    
    int threadNum = atoi(argv[2]);
    int arrayLen = atoi(argv[1]);
    //int iteration = arrayLen<=(1<<15)?100:10;
    int iteration = 1;
    int* array=(int*)malloc(arrayLen*sizeof(int));

    
    
    
    double timeTotal=0;
    for(int currIter=0; currIter<iteration; currIter++){
        // srand(time(0));
        // for(int i=0; i < arrayLen; i++) {
        //     array[i]=(rand()%1000);
        // }
        for(int i=0; i < arrayLen; i++) {
            array[i]=100-i;
        }

        printf("random array:\n");
        for(int i=0; i < arrayLen; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        double t1= omp_get_wtime();
        if(threadNum==0)
            sampleSort(array, 0, arrayLen-1);
        else
            sampleSort_parallel(array, 0, arrayLen-1, threadNum);
        double t2= omp_get_wtime();
        timeTotal=timeTotal+t2-t1;

        printf("partitioned array:\n");
        for(int i=0; i < arrayLen; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
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