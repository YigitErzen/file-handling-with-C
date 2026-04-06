#include <stdio.h>

int main(){
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL){
        perror("Error file opening!");
    }
    else{
        int ch_code = '\0';
        while(ch_code = fgetc(fp) != EOF){
            print("%c", ch_code);
        }
    }
    return 0;
}