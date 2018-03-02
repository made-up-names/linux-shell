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
			jno[pid]=-1;
			njobs--;
			free(bg[pid]);
			bg[pid]=NULL;
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
				jno[pid]=-1;
				njobs--;
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

