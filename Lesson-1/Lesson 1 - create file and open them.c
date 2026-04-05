#include <stdio.h>

int main() {
    
    FILE* file_pointer = NULL;

    file_pointer = fopen("yigit.txt", "w");

    if(file_pointer == NULL){
        printf("Error while opening the file");

    }
    else{
        printf("File opened succesfully");
        fclose(file_pointer);
    }

    return 0;
}