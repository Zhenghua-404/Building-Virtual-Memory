#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
FILE* ram[10];
int count=-1;

int addToRAM(FILE* p){ //add the file pointer to the next available cell in ram[]
    ram[count+1]=p;
    count++;
    return count;
}

void delete(int i){
    fclose(ram[i]);
    ram[i]=NULL;
}

void clearCount(){
    count=-1;
}
