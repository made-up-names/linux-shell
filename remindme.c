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
int remindme(int argc,char* args[])
{
	if(argc<3)
	{
		printf("syntax error: usage = remindme noofsec message\n");
		return 0;
	}
	pid_t pid,w;
	pid=fork();
	if(pid<0)
		printf("no child process created\n");
	else if(pid==0)
	{
		sleep(atoi(args[1]));
		printf("Reminder:");
		for(int i=2;i<argc;i++)
			printf("%s ",args[i]);
		printf("\n");
	}
//	exit(EXIT_SUCCESS);
	return 0;

}

