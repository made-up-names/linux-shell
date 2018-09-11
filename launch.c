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
int bg1(int argc,char* args[]);
int fg1(int argc,char* args[]);


int launch(int argc,char* args[],int fg)
{

	pid_t pid,w;
	int wstatus;
	if(fg){

		// not forking i.e not part of the foreground process group doubt with remindme ? 
		signal(SIGCHLD,SIG_DFL);
		if(strcmp(args[0],"cd")==0)
			return(cd(argc,args));
		else if(strcmp(args[0],"pwd")==0)
			return(pwd(args));
		else if(strcmp(args[0],"echo")==0)
			return(echo(args));
		else if(strcmp(args[0],"pinfo")==0) //not forking pinfo since if pinfo is typed should
			return(pinfo(argc,args));   //show the process info of the "shell"proces
		else if(strcmp(args[0],"remindme")==0) //doing remindme is a backgrund process if remindme exits shows the prompt
			return(remindme(argc,args)); //not forking twice // child of remindme cannot be exited through ctrlc since signt is ignored // remindme & "child of child will happen" // 
		else if(strcmp(args[0],"setenv")==0)
			return(lsetenv(argc,args));
		else if(strcmp(args[0],"unsetenv")==0) //shell env
			return(lunsetenv(argc,args));
		else if(strcmp(args[0],"jobs")==0) 
			return(jobs(argc,args));
		else if(strcmp(args[0],"kjob")==0)
			return(kjob(argc,args));
		else if(strcmp(args[0],"overkill")==0)
			return(kjob(argc,args));
		else if(strcmp(args[0],"bg")==0)
			return(bg1(argc,args));
		else if(strcmp(args[0],"fg")==0)
			return(fg1(argc,args));
		printf("here\n");
		signal(SIGCHLD,exithandler);

	}
	pid=fork();

	if(pid==0)
	{
		if(fg)
		{
			printf("fg=%d\n",fg);
			fore=getpid();
			printf("fore=%d\n",fore);
			signal(SIGINT,SIG_DFL);
			signal(SIGTSTP,ctrlzhandler);
			//signal(SIGTSTP,sigtstphandler); //add into bg thing
		}
		else
		{
			printf("fg=%d\n",fg);
			signal(SIGINT,SIG_IGN);
			signal(SIGTSTP,SIG_IGN);
		//	signal(SIGTSTP,SIG_IGN);
		}
		//signal(SIGTSTP,SIG_DFL);
		int exitstatus=0;
		printf("new proces created\n");
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
			exitstatus=bg1(argc,args);
		else if(strcmp(args[0],"fg")==0)
			exitstatus=fg1(argc,args);
		else if(execvp(args[0],args)==-1)
		{
			perror("bash:execution error");
			exit(EXIT_FAILURE);
		}
		printf("%d\n",exitstatus);
		exit(exitstatus);
	}
	else if(pid<0)
		perror("bash:fork error");
	else if(pid>0)
	{
		if(fg)
		{
			
				w=waitpid(pid,&wstatus,WUNTRACED|WCONTINUED); 
				printf("some statement\n");
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
				else if(w<0)
					perror("waitpid");
	
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
			printf("njobs before incrementing in launch%d\n",njobs);
			njobs++;
			jno[njobs]=pid;
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
	printf("%s %s\n",args[1],getenv(args[1]));
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
	for(int i=1;i<=njobs;i++)
	{
		if(jno[i]<0)
			continue;
		if(bg[jno[i]]!=NULL)
		{
			printf("what the bullshit %s\n",bg[jno[i]]);
			printf("pid=%d,bg[pid]=%s\n",jno[i],bg[jno[i]]);
			char procname[LIMIT];
			sprintf(procname,"/proc/%d/stat",jno[i]);
			int fd=open(procname,O_RDONLY);
			if(fd<0)
			{
				//fprintf(stderr,"no such process exists\n");
				jno[i]=-1;
				free(bg[jno[i]]);
				bg[jno[i]]=NULL;
				close(fd);
				continue;
			}
			char readfile[LIMIT];
			char state[LIMIT];
			if(read(fd,readfile,LIMIT-1)<0)
			{
				strcpy(state,"could not get this process state\n");
				printf("[%d] %s %s %d\n",i,state,bg[jno[i]],jno[i]);

			}
			strtok(readfile," ");
			strtok(NULL," ");
			strcpy(state,strtok(NULL," "));
			if(strcmp(state,"T")==0)
				strcpy(state,"Stopped");
			else
				strcpy(state,"Running");
			printf("[%d] %s %s %d\n",i,state,bg[jno[i]],jno[i]);
			close(fd);
		}
	}
	int j=njobs;
	while(j&&(jno[j]<0))
		j--;
	njobs=j;
	printf("adsjfjskf\n");
	return 0;
}
int kjob(int argc,char * args[])
{
	int exitstatus=0;
	int flag=1;
	if(strcmp(args[0],"kjob")==0)
	{
		printf("camehere\n");
		if(argc!=3)
		{
			printf("Syntax error: kjob <job number> <signal no>\n");
			return 1;
		}
		flag=0;
		int jno1=atoi(args[1]);
		int sig=atoi(args[2]);
		//if job is stopped // queue all signals // once sigcont is sent // perform all of them , 
		//if sigkill then empty the queue 
		//queue for each job number
		// first implementing ignore all signals except sigcont and sigkill
		int pid=jno[jno1];

			char procname[LIMIT];
			sprintf(procname,"/proc/%d/stat",pid);
			int fd=open(procname,O_RDONLY);
			if(fd<0)
			{
				fprintf(stderr,"no such process exists\n");
				jno[jno1]=-1;
				if(bg[pid])
				free(bg[pid]);
				bg[pid]=NULL;
				close(fd);
				return 1;
				
			}
			char readfile[LIMIT];
			char state[LIMIT];
			if(read(fd,readfile,LIMIT-1)<0)
			{
				strcpy(state,"could not get this process state\n");
				//don't know how to handle this case

			}
			strtok(readfile," ");
			strtok(NULL," ");
			strcpy(state,strtok(NULL," "));
			if(strcmp(state,"T")==0)
			{
				if(sig!=SIGCONT || sig !=SIGKILL)
				{
					printf("Process is Stopped can only take sigcont or sigkill signals\n");
					return 1 ;
				}
			}
		printf("camehere\n");
							//show the process info of the "shell"proces
		if(kill(jno[jno1],sig)<0)
		{
			perror("Error killing the job:");
			exitstatus=1;
		}
/*		if(sig==SIGKILL || sig == SIGTERM)
		{
			jno[jno1]=-1;
			if(bg[pid])
			{
				free(bg[pid]);
				bg[pid]=NULL;
			}
	printf("something\n");
	int j=njobs;
	while(j&&(jno[j]<0))
		j--;
	njobs=j;

		}*/
		printf("abcd camehere\n");
		return 0;
	}

	for(int i=1;i<=njobs;i++)
	{
		printf("%d\n",njobs);
		int pid=jno[i];
		printf("pid=%d\n",pid);
		printf("bg[pid]==%s\n",bg[pid]);
		if(bg[pid]!=NULL)
		{
		printf("pid=%d\n",pid);

			char procname[LIMIT];
			sprintf(procname,"/proc/%d/stat",jno[i]);
			int fd=open(procname,O_RDONLY);
			if(fd<0)
			{
				//fprintf(stderr,"no such process exists\n");
		printf("pid=%d\n",pid);
				jno[i]=-1;
				free(bg[jno[i]]);
				bg[jno[i]]=NULL;
				close(fd);
				continue;
			}
			char readfile[LIMIT];
			char state[LIMIT];
			if(read(fd,readfile,LIMIT-1)<0)
			{
				strcpy(state,"could not get this process state\n");
				//don't know how to handle this case

			}
			strtok(readfile," ");
			strtok(NULL," ");
			strcpy(state,strtok(NULL," "));

			close(fd);
			if(flag){
			int signo=SIGTERM;
			if(strcmp(state,"T")==0)
			{
		printf("here important macha pid=%d\n",pid);
				signo=SIGKILL;
				
			}
				if(kill(pid,signo)<0) // useSIGSTOP if stopped process
				{
					printf("pid=%d ",pid);
					perror("Error killing this job:");
					exitstatus=1;
					continue;
				}
			}
		}
	}
	printf("something\n");
	int j=njobs;
	while(j&&(jno[j]<0))
		j--;
	njobs=j;
	return exitstatus;
}
int bg1(int argc,char* args[])
{
	if(argc!=2)
	{
		printf("Syntax error: bg <job>\n");
		return 1;
	}
	int jno1=atoi(args[1]);
	int pid=jno[jno1];
	if(kill(pid,18)<0)
	{
		printf("pid=%d ",pid);
		perror("Error continuing this job:");
		return 1;
	}
	return 0;
}
int fg1(int argc,char* args[])
{
	if(argc!=2)
	{
		printf("Syntax error: fg <job no>\n");
		return 1;
	}
	int jno1=atoi(args[1]);
	int pid=jno[jno1];
	if(kill(pid,SIGCONT)<0)
	{
		printf("pid=%d ",pid);
		perror("Error continuing this job:");
	}
	int status;
	printf("%d\n",pid);
	fore=pid;
	if(waitpid(pid,&status,WUNTRACED)<0)
	{
		perror("Not able to execute the command");
		return 1;
	}
	//bring the damn thing to the front
	printf("abc\n");
	free(bg[pid]);
	bg[pid]=NULL;
	return 0;
}
