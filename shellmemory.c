#include<stdlib.h>
#include<string.h>
#include<stdio.h>
char* mem[1000][2];
int pos=-1;

void set(char* name, char* val){
	int find=0;
	for(int i=0;i<=pos;i++){
		if(strcmp(mem[i][0],name)==0){
			mem[i][1]=strdup(val);
			find=1;
		}
	}
	if(find==0){
		mem[pos+1][0]=strdup(name);
		mem[pos+1][1]=strdup(val);
		pos++;	
	}	
	return;
}

void print(char* name){
	for(int i=0;i<=pos;i++){
		if(strcmp(mem[i][0],name)==0){
			printf("%s\n",mem[i][1]);
			return;	
		}
	}
	printf("Variable does not exist\n");
	return;
}
