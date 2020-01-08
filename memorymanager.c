#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pcb.h"
#include "kernel.h"
#include "memorymanager.h"

int fileNumber=0;
//return 1 if successful, 0 otherwise
int launcher(FILE* p){
    char name [100]="touch ./BackingStore/";
    char numName [100];
    sprintf(numName,"%d",fileNumber);
    strcat(name,"P");
    strcat(name,numName);
    strcat(name,".txt");
    system(name);
    char filename [100]="./BackingStore/";
    strcat(filename,"P");
    strcat(filename,numName);
    strcat(filename,".txt");
    strcpy(tail->nameString,filename);
    FILE* diskFile=fopen(filename,"r+");
    char c;
    c = fgetc(p);
    while(c != EOF){
        fputc(c, diskFile);
        c = fgetc(p);
    }
    //fputc(EOF, diskFile);
    fclose(p);
    fseek(diskFile,0,SEEK_SET);
    tail->PC=diskFile;
    fileNumber++;
    int totalPages=tail->pages_max;
    if(totalPages==0){
        return 1;
    }
    if(totalPages<=1){
        FILE* curPage=findPage(0,diskFile);
        int frameNumber=findFrame(curPage);
        updatePageTable(tail,0,frameNumber,-1);
        fseek(p,0, SEEK_SET);
    }
    else{
        FILE* curPage=findPage(0,diskFile);
        int frameNumber=findFrame(curPage); 
        updatePageTable(tail,0,frameNumber,-1);
        curPage=findPage(1,diskFile);
        frameNumber=findFrame(curPage);
        updatePageTable(tail,1,frameNumber,-1);
        fseek(p,0, SEEK_SET);
    }
    return 1; //successful
}
int countTotalPages(FILE* f){
    int count=0;
    char c=getc(f);
    while(1){
        if(c==EOF){
            fseek(f,0, SEEK_SET);
            return ((count+1)%4==0) ? (count+1)/4 : (count+1)/4+1 ;
        }
        if(c=='\n'){
            count++;
        }
        c=getc(f);
    }
    return -1; // error
}
//
FILE* findPage(int pageNumber, FILE* f){
    FILE* fp2=fdopen(dup(fileno(f)),"r+");
    char text [1000];
    for(int i=0;i<4*pageNumber;i++){
        fgets(text,999,fp2);
    }
    return fp2;
}
int findFrame(FILE* page){
    int frame=-1;
    for(int i=0;i<10;i++){
        if(ram2[i]==NULL){
            frame=i;
            break;
        }
    }
    if(frame==-1){
        return -1;
    }
    updateFrame(frame,-1,page);
    return frame;
}
int findVictim(struct PCB *p){
    int rad=rand()%10; printf("%d",rad);
    int num=rad;
    do{
        int check=0;
        for(int i=0;i<10;i++){
            if(p->pageTable[i]!=num){
                check++;
            }
        }
        if(check==10){
            return num;
        }
        num=(num+1)%10;
    }while(num!=rad);
    return -1; //error return -1
}
int updateFrame(int frameNumber, int victimFrame, FILE* page){
    int frame;
    if(frameNumber==-1){ 
        frame=victimFrame;
    }else{
        frame=frameNumber;
    } 
    ram2[frame]=page;
    return 0;//successful
}
int updatePageTable(struct PCB* p, int pageNumber, int frameNumber, int victimFrame){
    if(frameNumber==-1){
        p->pageTable[pageNumber]=victimFrame;
    }else{
        p->pageTable[pageNumber]=frameNumber;
    }
    return 0; //successful
}
