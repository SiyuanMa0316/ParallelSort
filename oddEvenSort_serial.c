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
        if(phase%2==0){
            for(int i=0; i+1<LENGTH; i+=2){
                if(array[i]>array[i+1]){
                    int temp = array[i];
                    array[i]=array[i+1];
                    array[i+1]=temp;
                }
            }
        }
        else{
            for(int i=1; i+1<LENGTH; i+=2){
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