#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"shell.h"
int parsecommand(char* command,char* args[])
{
	// parse the command into args such that all tabs, new lines and spaces are ignored whenever there isn't any "
	char* delimiter=" \t\r\n\a\'\"";
	int i=0;
	char* arg=(char*)malloc(LIMIT*sizeof(char));
	arg=strtok(command,delimiter);
	args[i]=(char*)malloc(LIMIT*sizeof(char));
	strcpy(args[i],arg);

	for(i=1;args[i-1]!=NULL;i++)
	{
		arg=strtok(NULL,delimiter);
		if(arg)
		{
			args[i]=(char*)malloc(LIMIT*sizeof(char));
			strcpy(args[i],arg);
		}
	}
	args[i-1]=NULL;
	free(arg);
	return i-1;
}

