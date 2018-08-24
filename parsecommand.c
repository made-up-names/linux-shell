#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"shell.h"
int parsecommand(char* command,char* args[],char *inputredir, char* outputredir, int* isappend)
{
	// parse the command into args such that all tabs, new lines and spaces are ignored whenever there isn't any "
	char* delimiter=" \t\r\n\a\'\"";
	int i=0;
	char* arg=(char*)malloc(LIMIT*sizeof(char));
	arg=strtok(command,delimiter);
	args[i]=(char*)malloc(LIMIT*sizeof(char));
	strcpy(args[i],arg);
	int flag=0;
	for(i=0;arg!=NULL;)
	{
		i++;
		arg=strtok(NULL,delimiter);
		if(arg&&strcmp(arg,"<")==0)
		{
			flag=1;	
		}
		else if(arg&&strcmp(arg,">")==0)
		{
			flag=2;
			*isappend=0;
		}
		else if(arg&&strcmp(arg,">>")==0)
		{
			flag=2;
			*isappend=1;
		}
		else if(arg&&flag==2)
		{
			strcpy(outputredir,arg);
		}
		else if(arg && flag==1)
		{
			strcpy(inputredir,arg);
		}
		else if(arg &&flag==0)
		{
			args[i]=(char*)malloc(LIMIT*sizeof(char));
			strcpy(args[i],arg);
		}
	}
	args[i]=NULL;
	free(arg);
	return i;
}
