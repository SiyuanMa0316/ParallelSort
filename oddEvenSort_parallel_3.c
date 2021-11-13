#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4
#define MAX_LENGTH 100000
#define INTERVAL 10000
#define REPEAT 10
#define START 10000

int main(){

    //file setup
    FILE *outfile;
    outfile = fopen("datapoints/4threads.txt","w");
    if(outfile == NULL){
        printf("Error: file did not open!\n");
        exit(1);
    }
    
    //vars
    int array[MAX_LENGTH];
    double start_time, run_time, avg_runtime;
    double runtime[REPEAT];
    omp_set_num_threads(NUM_THREADS);
    
    
    //iterate over different array lengths
    for(int length=START; length<=MAX_LENGTH; length+=INTERVAL){
        avg_runtime = 0.0;
        for(int run=0; run<REPEAT; run++){
            //initialization
            for(int j=0; j<length; j++){
                array[j]=length-j;
            }
            start_time = omp_get_wtime();
            for(int phase=0; phase<length; phase++){
                int i;
                if(phase%2==0){
                    #pragma omp parallel for 
                    for(i=0; i<length-1; i+=2){
                        if(array[i]>array[i+1]){
                            int temp = array[i];
                            array[i]=array[i+1];
                            array[i+1]=temp;
                        }
                    }
                }
                else{
                    #pragma omp parallel for 
                    for(i=1; i<length-1; i+=2){
                        if(array[i]>array[i+1]){
                            int temp = array[i];
                            array[i]=array[i+1];
                            array[i+1]=temp;
                        }
                    }
                }
            }
            runtime[run] = omp_get_wtime() - start_time;
        }
        //calculate runtime
        for(int r=0; r<REPEAT;r++){
            avg_runtime += runtime[r];
        }
        avg_runtime = avg_runtime/REPEAT;
        fprintf(outfile,"%d;%f\n",length,avg_runtime,outfile);
        printf("Interval %d of %d\n",(length/INTERVAL), (MAX_LENGTH/INTERVAL));
    }
    fclose(outfile);
}   