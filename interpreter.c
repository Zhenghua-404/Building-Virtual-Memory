#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "memorymanager.h"


int interpreter(char* words [], int length);
void help(){
	printf("COMMAND            DESCRIPTION\n");
	printf("help               Displays all the commands\n");
	printf("quit               Exits/terminates the shell with \"Bye!\"\n");
	printf("set VAR STRING     Assigns a value to shell memory\n");
	printf("print VAR          Prints the STRING assigned to VAR\n");
	printf("run SCRIPT.TXT     Executes the file SCRIPT.TXT\n");
    printf("exec p1 p2 p3      Executes concurrent programs\n                   $ exec prog.sh prog2.sh\n");
}


void quit(){
	printf("Bye!\n");
}


int run(char* fname){
	char line[1000];
	FILE* f=fopen(fname,"rt");
	if(f==NULL){
		printf("Script not found\n");
		return 0;
	}
	while(feof(f)==0){
		char* words[1000];
		if(fgets(line,999,f)==NULL){
			break;
		}
		int length=parse(words,line);
		if(interpreter(words,length)==-1){
			return -1;
		}
	}
	return 0;
}


int exec(char* words [], int length){
    for(int i=1;i<length;i++){
        FILE* f=fopen(words[i],"rt");
        if(f==NULL){ 
            printf("Script not found\n");
            return 0;
        }
        myinit(f); 
        launcher(f);
    }
    return scheduler();
}


int interpreter(char* words [], int length){
	if(strcmp(words[0],"help")==0 && length==1){
		help();	
		return 0;
	}
	else if(strcmp(words[0],"quit")==0 && length==1){
		quit();
		return -1;	
	}
	else if(strcmp(words[0],"set")==0 && length==3){
		set(words[1],words[2]);
		return 0;
	}
	else if(strcmp(words[0],"print")==0 && length==2){
		print(words[1]);
		return 0;
	}
	else if(strcmp(words[0],"run")==0 && length==2){
		if(run(words[1])==-1){
			return -1;
		}
		return 0;
	}
    else if(strcmp(words[0],"exec")==0 && (length==2 || length==3 || length==4)){
        return exec(words, length);
    }
	else{
		printf("Unknown command\n");
		return 0;
	}
}


