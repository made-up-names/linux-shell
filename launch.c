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
#include<fcntl.h>
#include"shell.h"
int lsetenv(int argc,char* args[]);
int lunsetenv(int argc,char* args[]);
int jobs(int argc,char* args[]);
int kjob(int argc,char* args[]);
int bg(int argc,char* args[]);
int fg(int argc,char* args[]);

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
		else if(strcmp(args[0],"setenv")==0)
			exitstatus=lsetenv(argc,args);
		else if(strcmp(args[0],"unsetenv")==0)
			exitstatus=lunsetenv(argc,args);
		else if(strcmp(args[0],"jobs")==0)
			exitstatus=jobs(argc,args);
		else if(strcmp(args[0],"kjob")==0)
			exitstatus=kjob(argc,args);
		else if(strcmp(args[0],"overkill")==0)
			exitstatus=kjob(argc,args);
		else if(strcmp(args[0],"bg")==0)
			exitstatus=bg(argc,args);
		else if(Strcmp(args[0],"fg")==0)
			exitstatus=fg(argc,args);
		else if(execvp(args[0],args)==-1)
		{
			perror("bash:execution error");
			exit(EXIT_FAILURE);
		}
		return exitstatus;
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
					printf("in launch here\n");


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
			char command[LIMIT];
			strcpy(command,"");
			for(int i=0;i<argc;i++)
			{
				strcat(command,args[i]);
				strcat(command," ");
			}
			strcpy(bg[pid],command);
			njobs++;
			jno[pid]=njobs;
		}
	}
	return 0;
}
int lsetenv(int argc,char* args[])
{
	if(argc<=1 || argc >3)
	{
		printf("Syntax error:setenv var [value]\n");
		return 1;
	}
	int err=0;
	if(argc==2)
		err=setenv(args[1],"",1);
	else
		err=setenv(args[1],args[2],1);
	if(err<0)
	{
		perror("Could not set the env variable");
		return 1;
	}
	return 0;
}
int lunsetenv(int argc,char * args[])
{
	if(argc!=2)
	{
		printf("Syntax error: unsetenv var\n");
		return 1;
	}
	int err=unsetenv(args[1]);
	if(err>=0)
		return 0;
	perror("Could not unset env variable:");
	return 1;

}
int jobs(int argc,char* args[])
{
	if(argc!=1)
	{
		printf("Syntax error: No command line arg required\n");
		return 1;
	}
	///think shreya think think think think think max
	//i'm printing out the sorted order but if the order is changed the just write the no of process
	for(int i=0;i<PIDLIMIT;i++)
	{
		if(bg[i]!=NULL)
		{
			//printf("what the bullshit %s\n",bg[i]);
			char procname[LIMIT];
			sprintf(procname,"/proc/%d/stat",i);
			int fd=open(procname,O_RDONLY);
			if(fd<0)
			{
				//fprintf(stderr,"no such process exists\n");
				free(bg[i]);
				bg[i]=NULL;
				close(fd);
				continue;
			}
			char readfile[LIMIT];
			char state[LIMIT];
			if(read(fd,readfile,LIMIT-1)<0)
			{
				strcpy(state,"could not get this process state\n");
				printf("[%d] %s %s %d\n",jno[i],state,bg[i],i);

			}
			strtok(readfile," ");
			strtok(NULL," ");
			strcpy(state,strtok(NULL," "));
			if(strcmp(state,"T")==0)
				strcpy(state,"Stopped");
			else
				strcpy(state,"Running");
			printf("[%d] %s %s %d\n",jno[i],state,bg[i],i);
			close(fd);
		}
	}
	return 0;
}
int kjob(int argc,char * args[])
{
	int flag=1;// kill all background process at once
	int sig=-1;
	int jno1=-1;
	if(strcmp(args[0],"kjob")==0)
	{
	if(argc!=3)
	{
		printf("Syntax error: kjob <job number>  <signo>\n");
		return 1;
	}
	sig=atoi(args[2]);
	jno1=atoi(args[1]);
		flag=0;
	}
	
	else
	{
		if(argc!=1){
		printf("Syntax error: overkill\n");
		return 1;
		}
	}

        for(int i=0;i<PIDLIMIT;i++)
        {
                if(bg[i]!=NULL)
                {
                        char procname[LIMIT];
                        sprintf(procname,"/proc/%d/stat",i);
                        int fd=open(procname,O_RDONLY);
                        if(fd<0)
                        {
                                //fprintf(stderr,"no such process exists\n");
                                free(bg[i]);
                                bg[i]=NULL;
                                close(fd);
                                continue;
                        }
                        close(fd);
			if(flag)
			{
				if(kill(i,9)<0)
					perror("Could not kill this bg process");
				continue;
			}
			if(jno[i]!=jno1)
			{	continue;
			}
			if(kill(i,sig)<0)
			{
				perror("invalid usage");
				return 1;
			}
			else
				return 0;
               
		 }
        }
	if(strcmp(args[0],"kjob")==0)
	printf("no such job exists\n");	
	
}
int bg(int argc,char* args[])
{
	if(argc!=2)
	{
		printf("Syntax error: bg <job>\n");
		return 1;
	}
	return 0;
}
