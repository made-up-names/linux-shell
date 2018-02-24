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
int launch(int argc,char* args[],int fg)
{
	
	pid_t pid,w;
	int wstatus;
	pid=fork();
	if(pid==0)
	{
		int exitstatus=0;
		if(strcmp(args[0],"cd")==0)
			exitstatus=cd(argc,args);
		else if(strcmp(args[0],"pwd")==0)
			exitstatus=pwd(args);
		else if(strcmp(args[0],"echo")==0)
			exitstatus=echo(args);
		else if(strcmp(args[0],"ls")==0)
			exitstatus=ls(argc,args);
		else if(strcmp(args[0],"pinfo")==0)
			exitstatus=pinfo(argc,args);
		else if(strcmp(args[0],"remindme")==0)
			exitstatus=remindme(argc,args);
		else if(strcmp(args[0],"clock")==0)
			exitstatus=lclock(argc,args);
		else if(execvp(args[0],args)==-1)
		{
			perror("bash:execution error");
			//exit(EXIT_FAILURE);
		}
	}
	else if(pid<0)
		perror("bash:fork error");
	else if(pid>0)
	{
		if(fg)
		{

			do
			{
				w=waitpid(pid,&wstatus,WUNTRACED|WCONTINUED); 
				if(w<0)
					perror("waitpid");
				if (WIFEXITED(wstatus))
				{
					printf("exited, status=%d\n", WEXITSTATUS(wstatus));
				//	printf("in launch here\n");
				} 
				else if (WIFSIGNALED(wstatus))
				{
				printf("killed by signal %d\n", WTERMSIG(wstatus));
				}
				else if (WIFSTOPPED(wstatus)) 
				{
					printf("stopped by signal %d\n", WSTOPSIG(wstatus));
				}
				else if (WIFCONTINUED(wstatus))
				{
					printf("continued\n");
				}
			}
			while(!WIFEXITED(wstatus)&&!WIFSIGNALED(wstatus));//foreground

		}
		else
		{
			printf("%s process pid =%d\n",args[0],pid);
			bg[pid]=(char*)malloc(LIMIT*(sizeof(char)));
			strcpy(bg[pid],args[0]);
		}
	}
	return 0;
}

