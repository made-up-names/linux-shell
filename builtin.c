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
int cd(int argc,char* args[])
{
	if(argc>2)
	{
		printf("syntax error\n");
		return 0;
	}

	// no directory
	if(argc==1)
	{
		int error=chdir(startwd);
		if(error<0)
			perror("Error");
		return error;
	}

	char tomove[LIMIT];
	strcpy(tomove,startwd);
	if(args[1][0]=='~')
		strcat(tomove,&args[1][1]);
	else
		strcpy(tomove,args[1]);
	int error=chdir(tomove);
	if(error<0)
		perror("Error");
	return error;
}
int pwd(char* args[])
{
	char cwd[LIMIT];
	getcwd(cwd,LIMIT-1);
	printf("%s\n",cwd);
	return 0;
}
int echo(char* args[])
{
	
	for(int i=1;args[i]!=NULL;i++)
	{
		if(args[i][0]!='$')
			printf("%s ",args[i]);
		else //if(getenv(&args[i][1]))
			printf("$%s=%s ",&args[i][1],getenv(&args[i][1]));
	
	}
	printf("\n");
	return 0;
}

