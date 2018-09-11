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
#include <fcntl.h>
#include"shell.h"


int redirectIO(char* inputredir,char* outputredir,int isappend);
int executewithpipes(char* command);
int main()
{
	fore=-1;
	for(int i=0;i<PIDLIMIT;i++)
		bg[i]=NULL;
	getcwd(startwd,LIMIT); // ~ directory : startwd
	signal(SIGCHLD,exithandler); //exit handler for bg processes
	signal(SIGINT,ctrlchandler);
	signal(SIGTSTP,ctrlzhandler);
	int njobs=0;
	char input[LIMIT];
	while(1)
	{
		printprompt(startwd);
		scaninput(input,LIMIT);		
		if(input==NULL)
		{
			continue;
		}
		// LIMIT is for strings or data arrays
		// LMT2 is for pointer arrays

		char *commands[LMT2]; // ; or & separated commands
		int fg[LIMIT];
		//for every malloc write an error statement
		int noofcommands=inputsplit(input,commands,";&",fg);  // malloc is used for commands array in this fn

		if(noofcommands<0)
		{
			printf("please provide <1024 commands only\n");
			continue;
		}

		int stdin_copy=dup(0);
		int stdout_copy=dup(1);

		for(int i=0;i<noofcommands;i++)
		{
			// if pipe exists in the command execute with pipe else break into args
				if(strchr(commands[i],'|')!=NULL)
				{
			executewithpipes(commands[i]);
			continue;
				}
				char* args[LMT2]; 
				char inputredir[LIMIT]="\0";
				char outputredir[LIMIT]="\0";
				int isappend=0;
				int noofargs=parsecommand(commands[i],args,inputredir,outputredir,&isappend); //args are malloced 
				redirectIO(inputredir,outputredir,isappend);
				execute(noofargs,args,fg[i]);
				for(int j=0;j<noofargs;j++)
				{
				if(args[j])
				free(args[j]);
				args[j]=NULL;
				}

				dup2(stdin_copy,0);
				dup2(stdout_copy,1);

		}
		for(int i=0;i<noofcommands;i++)
		{
			if(commands[i])
				free(commands[i]);
			commands[i]=NULL;
		}
		close(stdin_copy);
		close(stdout_copy);
	}
	return 0;
}
int redirectIO(char* inputredir,char* outputredir,int isappend)
{
	// did not implement stderr redirection
	// right now only works if there is a space between > and the file name
	// error otherwise
	if(strcmp(inputredir,"\0")!=0)
	{
		printf("inside redirect should not happen\n");
		int in=open(inputredir,O_RDONLY|O_CREAT);
		if(in>=0)
		{
			int dup2er=dup2(in,0);
			if(dup2er<0)
				perror("Error redirecting inputfile");
			close(in);
		}
		else
		{
			perror("could not open read file");
		}

	}
	if(strcmp(outputredir,"\0")!=0)
	{
		int out;
		printf("inside redirect should not happen\n");
		if(isappend==1)
			out=open(outputredir,O_WRONLY|O_CREAT|O_APPEND,0644);
		else
			out=open(outputredir,O_WRONLY|O_CREAT|O_TRUNC,0644);
		if(out<0)
			perror("Could not open write file");
		else
		{
			int dup2er=dup2(out,1);
			if(dup2er<0)
				perror("Error redirecting outputfile");
			close(out);
		}
	}
	return 0;

}
int executewithpipes(char* command)
{
	//execute the command from the start to the first pipe
	int noofpipecommands=0;
	char *pipedcommands[LMT2];
	char* pipe1=strtok(command,"|");
	while(pipe1)
	{
		pipedcommands[noofpipecommands]=(char*)malloc(LIMIT*sizeof(char));
		strcpy(pipedcommands[noofpipecommands],pipe1);
		pipe1=strtok(NULL,"|");
		noofpipecommands++;
	}
	if(noofpipecommands==1 && strchr(command,'|')!=NULL)
	{
		printf("There has to be atleast 2 commands to use a pip\n");
		return 0;
	}

	int stdin_copy=dup(0);
	int stdout_copy=dup(1);
	int readfd=dup(stdin_copy);
	int writefd=-1;
	for(int i=0;i<noofpipecommands;i++)
	{

		char* args[LMT2]; 
		char inputredir[LIMIT];
		strcpy(inputredir,"\0");
		char outputredir[LIMIT];
		strcpy(outputredir,"\0");
		int isappend=0;
		int noofargs=parsecommand(pipedcommands[i],args,inputredir,outputredir,&isappend); // break the command into args

		// if conditions for input redirection are irrelevant right now
		if( strcmp(inputredir,"\0")==0)
		{
			if(i)
			{
				int dup2er=dup2(readfd,0);
				if(dup2er<0)
					perror("Error input redirecting in pipes ");
				close(readfd);
			}

		}
		if(strcmp(outputredir,"\0")==0)
		{
			if(i!=noofpipecommands-1)
			{
				int pipefd[2];
				int piperror=pipe(pipefd);
				if(piperror<0)
				{
					perror("Unable to create pipes");
					continue;
				}
				writefd=pipefd[1];
				readfd=pipefd[0];
			}
			else
			writefd=dup(stdout_copy);
		int dup2er=dup2(writefd,1);
		if(dup2er<0)
			perror("Error output redirecting in pipes ");
		close(writefd);
		}

		//for input output redirection in the pipe commands (irrelevant right now)
		redirectIO(inputredir,outputredir,isappend);
		execute(noofargs,args,1);
		for(int j=0;j<noofargs;j++)
		{
			if(args[j])
				free(args[j]);
			args[j]=NULL;
		}
		dup2(stdin_copy,0);
		dup2(stdout_copy,1);


	}
	for(int i=0;i<noofpipecommands;i++)
	{
		if(pipedcommands[i])
			free(pipedcommands[i]);
		pipedcommands[i]=NULL;
	}
	close(stdout_copy);
	close(stdin_copy);

}
