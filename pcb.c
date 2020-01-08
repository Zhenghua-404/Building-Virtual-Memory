#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pcb.h"
#include "memorymanager.h"

/*
struct PCB{
	FILE* PC;
};
*/
struct PCB* makePCB(FILE* p){
    struct PCB* pcb=(struct PCB*)malloc(sizeof(struct PCB));
	pcb->PC=p;
    pcb->pages_max=countTotalPages(p);
    pcb->PC_page=0;  //initialize to 0
    pcb->PC_offset=0;  //initialize to 0
    for(int i=0;i<10;i++){
        pcb->pageTable[i]=-1;
    }
	return pcb;
}

