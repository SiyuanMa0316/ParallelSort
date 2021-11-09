#include <stdio.h>
#include <omp.h>

#define MAX_THREADS 4
#define LENGTH 50000

int main(){
	double start_time, run_time;
	int array[LENGTH];

    // printf("array before sort:\n");
    // for(int i=0; i<LENGTH; i++){
    //     printf("%d ",array[i]);
    // }

	// parallelizing loops with different thread counts
	// probably increment with j*=2 for MAX_THREADS >= 16
    for(int j=1;j<=MAX_THREADS ;j++){
    	//initializaiton
    	printf("Parallel iterations with %d thread(s):\n",j);
	    for(int i=0; i<LENGTH; i++){
	        array[i]=LENGTH-i;
	    }
		omp_set_num_threads(j);
		start_time = omp_get_wtime();

	    for(int phase=0; phase<LENGTH; phase++){
	        #pragma omp parallel
	        {   
	            int threadID = omp_get_thread_num(),
	                numthreads = omp_get_num_threads();
	            if(phase%2==0){
	                for(int i=threadID; i<LENGTH; i+=numthreads){
	                    if(array[i]>array[i+1]){
	                        int temp = array[i];
	                        array[i]=array[i+1];
	                        array[i+1]=temp;
	                    }
	                }
	            }
	            else{
	                for(int i=threadID+1; i<LENGTH; i+=numthreads){
	                    if(array[i]>array[i+1]){
	                        int temp = array[i];
	                        array[i]=array[i+1];
	                        array[i+1]=temp;
	                    }
	                }
	            }
	        }
	    }
	    run_time = omp_get_wtime() - start_time;
		printf("%d elements sorted in %f seconds.\n\n",LENGTH,run_time);
	}
	// printf("array after sort:\n");
	// for(int i=0; i<LENGTH; i++){
	//     printf("%d ",array[i]);
	// }
}	