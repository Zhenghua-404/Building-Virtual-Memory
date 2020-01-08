#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "interpreter.h"
//#define CLEARBUF() char ch; while ((ch=getchar()) != '\n' && ch!=EOF && ch!='\s');
//void helper1();

int parse(char* words[],char input[]){
	int w=0;
	char temp[200];
	int i=0;
	for(i=0; input[i]==' ' && i<1000; i++);
	while(input[i]!='\r' && input[i]!='\n' && i<1000){
		if(input[i]==' '){
			i++;
			continue;
		}
		int b=0;
		for(b=0 ;input[i]!='\r' && input[i]!='\n' &&input[i]!='\r' && input[i]!=' ';i++,b++){
			temp[b]=input[i];	
		}	
		temp[b]='\0';
		words[w]=strdup(temp);
		w++;
	}
	return w;
}


