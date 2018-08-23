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
#include "shell.h"
int execute(int argc,char* args[],int fg)
{
	if(!args[0])
		return 0;
	int exitstatus=0;
	
	if(strcmp(args[0],"quit")==0||strcmp(args[0],"exit")==0)
		exit(EXIT_SUCCESS);
	else exitstatus=launch(argc,args,fg);
	return exitstatus;
}

