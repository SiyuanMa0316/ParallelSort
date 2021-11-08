#include<omp.h>
#include<stdio.h>

#define LENGTH 10
int main(){
    int array[LENGTH];
    //initializaiton
    for(int i=0; i<LENGTH; i++){
        array[i]=LENGTH-i;
    }
    printf("array before sort:\n");
    for(int i=0; i<LENGTH; i++){
        printf("%d ",array[i]);
    }
    
    for(int phase=0; phase<LENGTH; phase++){
        int i;
        if(phase%2==0){
            #pragma omp parallel for 
            for(i=0; i<LENGTH-1; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
        else{
            #pragma omp parallel for 
            for(i=1; i<LENGTH-1; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
    }

    printf("\narray after sort:\n");
    for(int i=0; i<LENGTH; i++){
        printf("%d ",array[i]);
    }

    printf("\n");
}