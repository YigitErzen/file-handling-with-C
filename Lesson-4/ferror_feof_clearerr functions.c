#include <stdio.h>

int main(){
    FILE *  fp = fopen("input.txt", "r");
    if (fp == NULL){
        perror("fopen");
        return 1;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }

    if (feof(fp)){
        printf("\nComplete File is read");
    }

    if (ferror(fp)){
        printf("Error occured while reading from the file");
    }

    clearerr(fp);

    if (ferror(fp)){
        printf("Error occured while reading from the file");
    }

    fclose(fp);
    return 0;
}