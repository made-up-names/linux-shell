#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<dirent.h>
#include<errno.h>
#include<signal.h>
#include"shell.h"
int inputsplit(char * input,char * commands[],char* delimiter,int* fg)
{
	// split input into commands separated by ';' and '&'
	// if command[i] has '&' to the right => fg[i]=0; else 1;

	// split the input using ';' and '&'
	//now fill in the values of fg array
	//search for ';' or '&' in the input string if you find '&' fg[i]=0 else 1
	int i=0;
	for(int j=0;input[j]!='\0';j++)
	{
		if(input[j]=='&')
			fg[i++]=0;
		else if(input[j]==';')
			fg[i++]=1;

	}
	char *dummy=strtok(input,delimiter);
	int noofcommands=0;
	if(dummy)
	{
		commands[0]=(char*)malloc(LIMIT*sizeof(char));
		strcpy(commands[0],dummy);
		noofcommands++;
	}
	for(int i=1;dummy!=NULL;i++)
	{
		dummy=strtok(NULL,delimiter);
		if(dummy)
		{
			commands[i]=(char*)malloc(LIMIT*sizeof(char));
			strcpy(commands[i],dummy);
			noofcommands++;
		}
			
	}
	if(i<noofcommands)
	{
		//printf("here i=%d c=%d\n",i,noofcommands);
		fg[i++]=1;
	}
	if(i!=noofcommands)
	{
		char error[LIMIT]="Error occured during splitting of commands\n";
		fprintf(stderr,"Error occured during splitting of commands\n");
		return -1;
	}
	return noofcommands;
}

