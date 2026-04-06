#include <stdio.h>

int main(){
    char data[] = "Hello World!";
    FILE *fp = fopen("output.txt", "w");
    if(fp == NULL)
    {
        perror("Error opening file");
    }
    else
    {
        /*printf("%c",fputc('A', fp));
        fputc(66, fp);
        fputc('C', fp);
        fclose(fp);*/

        int index = 0; 
        char ch = '\0';
        while (data[index] != '\0'){
            ch = fputc(data[index], fp);
            printf("%c", ch);
            index++;
        }
        fclose(fp);
    }
    return 0;
}