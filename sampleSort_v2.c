#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<omp.h>
#define INT_MAX 2147483647
#define INT_MIN -2147483647-1

/*
compile: gcc -O0 -fopenmp sampleSort_v2.c -o sampleSort_v2
usage: ./sampleSort_v2 ARRAY_LEN THREAD_NUM
set THREAD_NUM=0 for serial without openMP. Otherwise parallel
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

void sampleSort(int* array, int start, int end){
    int bucketNum=16;
    //int pivotVal[bucketNum+1];
    int* pivotVal=(int*)malloc( (bucketNum+1)*sizeof(int));
    pivotVal[0]=INT_MIN;
    for(int i=1; i<bucketNum; i++){
        pivotVal[i]=array[start+i*(end-start)/bucketNum];
    }
    pivotVal[bucketNum]=INT_MAX;

    // printf("pivot values:\n");
    // for(int i=0; i<bucketNum+1; i++){
    //     printf("%d ", pivotVal[i]);
    // }
    // printf("\n");

    qsort(pivotVal, bucketNum+1, sizeof(int), compare_function);

    // printf("pivot values:\n");
    // for(int i=0; i<bucketNum+1; i++){
    //     printf("%d ", pivotVal[i]);
    // }
    // printf("\n");

    int** bucket=(int**)malloc(bucketNum*sizeof(int*));
    int* bucketSize=(int*)malloc(bucketNum*sizeof(int));
    int* bucketPointer=(int*)malloc(bucketNum*sizeof(int));
    for(int i=0; i<bucketNum; i++){
        bucketSize[i]=16;
        bucket[i]=(int*)malloc(bucketSize[i]*sizeof(int));
        bucketPointer[i]=0;
    }
    for(int i=0; i<bucketNum; i++){
        for(int j=0; j<bucketSize[i]; j++){
            bucket[i][j]=0;
        }
    }

    

    for(int i=start; i<=end; i++){
        int j=0;
        while(pivotVal[j]<=array[i]) j++;
        j--;     
        if(bucketPointer[j]==bucketSize[j]){
            bucketSize[j]=bucketSize[j]*2;
            int* temp=(int*)realloc(bucket[j], bucketSize[j] * sizeof(int));
            if(temp){
                bucket[j]=temp;
            }
            else{
                printf("no enough memory!");
                return;
            }
        }
        bucket[j][bucketPointer[j]]=array[i];
        bucketPointer[j]++;
    }

    // printf("bucket:\n");
    // for(int i=0; i<bucketNum; i++){
    //     for(int j=0; j<bucketSize[i]; j++){
    //         printf("%d ",bucket[i][j]);
    //     }
    //     printf("\n");
    // }

    int arrayIndex=0;
    for(int i=0; i<bucketNum; i++){
        memcpy(array + arrayIndex, bucket[i], bucketPointer[i]*sizeof(int));
        arrayIndex = arrayIndex + bucketPointer[i];
    }

    // printf("partitioned array:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    arrayIndex=0;
    for(int i=0; i<bucketNum; i++){
        qsort(array + arrayIndex, bucketPointer[i], sizeof(int), compare_function);
        arrayIndex = arrayIndex + bucketPointer[i];
    }
    // printf("sorted array:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
}

void sampleSort_parallel(int* array, int start, int end, int threadNum){

    double t1= omp_get_wtime();

    int bucketNum=threadNum;
    //int pivotVal[bucketNum+1];
    int* pivotVal=(int*)malloc( (bucketNum+1)*sizeof(int));
    pivotVal[0]=INT_MIN;
    for(int i=1; i<bucketNum; i++){
        pivotVal[i]=array[start+i*(end-start)/bucketNum];
    }
    pivotVal[bucketNum]=INT_MAX;

    // printf("pivot values:\n");
    // for(int i=0; i<bucketNum+1; i++){
    //     printf("%d ", pivotVal[i]);
    // }
    // printf("\n");

    qsort(pivotVal, bucketNum+1, sizeof(int), compare_function);

    // printf("pivot values:\n");
    // for(int i=0; i<bucketNum+1; i++){
    //     printf("%d ", pivotVal[i]);
    // }
    // printf("\n");

    int** bucket=(int**)malloc(bucketNum*sizeof(int*));
    int* bucketSize=(int*)malloc(bucketNum*sizeof(int));
    int* bucketPointer=(int*)malloc(bucketNum*sizeof(int));
    for(int i=0; i<bucketNum; i++){
        bucketSize[i]=end-start+1;
        bucket[i]=(int*)malloc(bucketSize[i]*sizeof(int));
        bucketPointer[i]=0;
    }
    // for(int i=0; i<bucketNum; i++){
    //     for(int j=0; j<bucketSize[i]; j++){
    //         bucket[i][j]=0;
    //     }
    // }

    double t_for_1= omp_get_wtime();

    int* whichBucket=(int*)malloc((end-start+1)*sizeof(int));

    omp_set_num_threads(bucketNum);
    #pragma omp parallel
    {
    #pragma omp single
        {printf("threadNum=%d\n",omp_get_num_threads());}
    #pragma omp for
    for(int i=start; i<=end; i++){
        
        int j=0;
        while(pivotVal[j]<=array[i]) j++;
        j--;
        whichBucket[i]=j;
        // if(bucketPointer[j]==bucketSize[j]){
        //     bucketSize[j]=bucketSize[j]*2;
        //     int* temp=(int*)realloc(bucket[j], bucketSize[j] * sizeof(int));
        //     if(temp){
        //         bucket[j]=temp;
        //     }
        //     else{
        //         printf("no enough memory!");
        //         return;
        //     }
        // }

    }
    }

for(int i=start; i<=end; i++){
        // if(bucketPointer[j]==bucketSize[j]){
        //     bucketSize[j]=bucketSize[j]*2;
        //     int* temp=(int*)realloc(bucket[j], bucketSize[j] * sizeof(int));
        //     if(temp){
        //         bucket[j]=temp;
        //     }
        //     else{
        //         printf("no enough memory!");
        //         return;
        //     }
        // }
        int j=whichBucket[i];
        bucket[j][bucketPointer[j]]=array[i];
        bucketPointer[j]++;
    }


    // printf("bucket:\n");
    // for(int i=0; i<bucketNum; i++){
    //     for(int j=0; j<bucketSize[i]; j++){
    //         printf("%d ",bucket[i][j]);
    //     }
    //     printf("\n");
    // }
    double t_for_2= omp_get_wtime();
    double t_memcpy_1= omp_get_wtime();

    int arrayIndex=0;
    for(int i=0; i<bucketNum; i++){
        memcpy(array + arrayIndex, bucket[i], bucketPointer[i]*sizeof(int));
        arrayIndex = arrayIndex + bucketPointer[i];
    }

    double t_memcpy_2= omp_get_wtime();

    // printf("partitioned array:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    int* partitionStart=(int*)malloc(bucketNum*sizeof(int));
    int accumulated=0;
    for(int i=0; i<bucketNum; i++){
        partitionStart[i] = accumulated;
        accumulated = accumulated + bucketPointer[i];
    }

    double t2= omp_get_wtime();

    omp_set_num_threads(bucketNum);
    #pragma omp parallel for
    for(int i=0; i<bucketNum; i++){
        qsort(array + partitionStart[i], bucketPointer[i], sizeof(int), compare_function);
    }
    // printf("sorted array:\n");
    // for(int i=start; i <= end; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    double t3= omp_get_wtime();
    printf("time: partitioning:%f(for loop %f, memcpy %f) , parallel sorting buckets:%f\n", t2-t1, t_for_2-t_for_1, t_memcpy_2-t_memcpy_1, t3-t2);
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
            sampleSort(array, 0, arrayLen-1);
        else
            sampleSort_parallel(array, 0, arrayLen-1, threadNum);
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