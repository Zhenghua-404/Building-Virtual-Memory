#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"
#include "interpreter.h"
#include "kernel.h"

FILE* ram2[10];
void addToReady(struct PCB* pcb);

struct PCB* head=NULL;
struct PCB* tail=NULL;
struct Node* nhead=NULL;
struct Node* ntail=NULL;

void myinit(FILE* p){
    struct PCB* pcb=makePCB(p);
	addToReady(pcb);
}

void addToReady(struct PCB* pcb){
	if(nhead==NULL){
		struct Node* cur=(struct Node*) malloc(sizeof(struct Node));
		cur->apcb=pcb;
		cur->next=NULL;
		nhead=cur;
		ntail=cur;
		head=cur->apcb;
		tail=cur->apcb;
	}
	else{
		struct Node* cur=(struct Node*) malloc(sizeof(struct Node));
		cur->apcb=pcb;
		cur->next=NULL;
        ntail->next=cur;
		ntail=cur;
		tail=cur->apcb;
        
	}
}

int scheduler(){
	cpu.quanta=2;
    while(nhead!=NULL){
        cpu.IP=head->PC;
        cpu.offset=head->PC_offset;
        int re=runCPU();
        if(re==1){
            addToReady(head);
            struct Node* garbage=nhead;
            nhead=nhead->next;
            free(garbage);
            head = nhead->apcb;
        }
        else if(re==0){
            char command[100]="rm ";
            strcat(command,head->nameString);
            system(command);
            struct Node* garbage=nhead;
            nhead=nhead->next;
            free(garbage->apcb);
            free(garbage);
            if (nhead != NULL) {
                head = nhead->apcb;
            } else {
                head = NULL;
                ntail=NULL;
                tail=NULL;
            }
        }
        else if(re==-1){
            //free the ready queue
            //free all PCBs
            //close file, free ram
            while (nhead) {
                struct Node* cur = nhead;
                nhead = nhead->next;
                free(cur->apcb);
                char command[100]="rm ";
                strcat(command,head->nameString);
                system(command);
                free(cur);
            }
            tail=NULL;
            ntail=NULL;
            head=NULL;
            clearCount();
            return -1;
        }
    }
    clearCount();
    return 0;
}

void boot(){
    for(int i=0;i<10;i++){
        ram2[i]=NULL;
    }
    system("rm -R BackingStore");
    system("mkdir BackingStore");
}

void main(){
    //char input [1000];
    boot();
    printf("Kernel 1.0 loaded!\n");
    printf("Welcome to Mavis shell!\n" );
    printf("Shell version 3.0 updated March 2019\n");
    int tag=0;
    while(1){
        char input [1000];
        if(tag==0){
            printf("$ ");
        }
        else if(tag==1){
            tag=0;
        }
        if(fgets(input,999,stdin)==NULL && isatty(0)==0){
            tag=1;
            freopen("/dev/tty","r",stdin);
            continue;
        }
        char* words[100];
        int length=parse(words,input);
        if(interpreter(words,length)==-1){
            return;
        }
    }
}
