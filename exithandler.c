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


void ctrlchandler(int sig)
{
	printf("\n");
	if(fore>0)
		kill(fore,SIGTERM);
	//signal(SIGCHLD,exithandler);
	printprompt(startwd);
	fflush(stdout);
}
void ctrlzhandler(int sig)
{
	printf("\n");
	printf("fore=%d\n",fore);
	if(fore<0)
	{
	printf("dsdjj\n");
	}

	char exelink[LIMIT] = "";
	char exepath[LIMIT] = "";
	sprintf(exelink,"/proc/%d/exe",fore);
	int exepathsize = readlink(exelink, exepath, LIMIT - 1);
	exepath[exepathsize] = '\0';

	if(fore>0)
	{
		bg[fore]=(char*)malloc(LIMIT*sizeof(char));
	strcpy(bg[fore],exepath);
	}
	printf("pid=%d,exelink=%s\n",fore,exelink);
	if(fore>0)
	{
		printf("fore=%d\n",fore);
		kill(fore,SIGTSTP);
	}
	//signal(SIGCHLD,exithandler);
	printprompt(startwd);
	fflush(stdout);
}




void exithandler(int sig)
{
	//for all ids in bg <bg size

	int wstatus;
	pid_t pid=wait(&wstatus);
	if(bg[pid]){
		char* p2=strchr(bg[pid],' ');
		int len=p2-bg[pid];
		char proc[LIMIT];
		strncpy(proc,bg[pid],len);
		proc[len]='\0';
		if (WIFEXITED(wstatus)) {
		fprintf(stderr,"%s process with %d ",proc,pid);
			if(WEXITSTATUS(wstatus))
				fprintf(stderr,"exited with status=%d\n",WEXITSTATUS(wstatus));
			else
				fprintf(stderr,"exited normally\n");
			for(int i=1;i<=njobs;i++)
			{
				if(jno[i]==pid)
					jno[i]=-1;
			}
			int j=njobs;
			while(j>0 && (jno[j]<0))
				j--;
			njobs=j;
			free(bg[pid]);
			bg[pid]=NULL;
			printf("file njobs=%d\n",njobs);
			printf("pid%d,bg[pid]=%s\n",pid,bg[pid]);
		}
		else if (WIFSIGNALED(wstatus))
		{
		fprintf(stderr,"%s process with %d ",proc,pid);
		fprintf(stderr,"killed by signal %d\n", WTERMSIG(wstatus));
		//if job doesnot exist then 
			char procname[LIMIT];
			sprintf(procname,"/proc/%d/status",pid);
			if(open(procname,O_RDONLY)<0)	
			{
				for(int i=1;i<=njobs;i++)
				{
					if(jno[i]==pid)
						jno[i]=-1;
				}
				int j=njobs;
				while(j>0 && (jno[j]<0))
					j--;
				njobs=j;
				printf("file njobs=%d\n",njobs);
				free(bg[pid]);
				bg[pid]=NULL;
			}

		}
		else if (WIFSTOPPED(wstatus)) 
		{
		fprintf(stderr,"%s process with %d ",proc,pid);
			fprintf(stderr,"stopped by signal %d\n", WSTOPSIG(wstatus));
		}
		else if (WIFCONTINUED(wstatus))
		{
		fprintf(stderr,"%s process with %d ",proc,pid);
			fprintf(stderr,"continued\n");
		}
		printprompt(startwd);
		fflush(stdout);
	}


}

