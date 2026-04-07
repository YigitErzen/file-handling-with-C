#include<stdio.h>

int main(){
    int age;
    float weight;
    char name[6];

    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL){
        perror("fopen");
        return 1;
    }
    
    if( fscanf(fp, "%d %f %24[^\n]",&age, &weight, name) == 3){
        printf("age: %d weight: %.2f name: %s", age, weight, name);
    }
    else{
        printf("Unable to read 3 values");
        printf("age: %d weight: %2.f name: %s", age, weight, name);
    }
}