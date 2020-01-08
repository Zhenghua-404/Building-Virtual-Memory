#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pcb.h"
#include "shell.h"
#include "interpreter.h"
#include "cpu.h"
#include "kernel.h"
#include "memorymanager.h"


int pageNumber=0;
struct PCB* findpcb(int frame){
    if(nhead==NULL){
        return NULL; //null check, won't happen
    }
    struct Node* p=nhead->next;
    while(p!=NULL){
        for(int i=0;i<10;i++){
            if(p->apcb->pageTable[i]==frame){
                pageNumber=i;
                return p->apcb;
            }
        }
        p=p->next;
    }
    return NULL;
}


int runCPU(){
    for(int i=0;i<cpu.quanta;i++){
        char* p=fgets(cpu.IR,999,cpu.IP);
        if(p==NULL){
            return 0;
        }
        char* words[100];
        int length=parse(words,cpu.IR);
        if(interpreter(words,length)==-1){ 
            return -1; 
        }
        cpu.offset++;
        if(cpu.offset==4){
            //page fault
            (head->PC_page)++;
            if(head->PC_page>=head->pages_max){
                return 0; //terminate
            }
            else{
                if(head->pageTable[head->PC_page]!=-1){
                    head->PC=ram2[head->pageTable[head->PC_page]];
                    head->PC_offset=0;
                }
                else{
                    fseek(head->PC, 0, SEEK_SET);
                    FILE* page=findPage(head->PC_page,head->PC);
                    int frame=findFrame(page);
                    if(frame==-1){
                        frame=findVictim(head);
                        updateFrame(-1,frame,page);
                        updatePageTable(head,head->PC_page,-1,frame);
                        if(findpcb(frame)!=NULL){
                            updatePageTable(findpcb(frame),pageNumber,-1,-1);
                        }//update victim
                    }
                    else{
                        updatePageTable(head,head->PC_page,frame,-1);
                    }
                    head->PC=ram2[frame];
                    head->PC_offset=0;
                }
            }
            return 1;
        }
    }
    head->PC=cpu.IP;
    head->PC_offset=cpu.offset;
    return 1;
}
