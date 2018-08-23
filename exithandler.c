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
void exithandler(int sig)
{
	//for all ids in bg <bg size

	int stat;
	pid_t pid=wait(&stat);
	if(bg[pid]){
		if (WIFEXITED(stat)) {
			fprintf(stderr,"%s process with %d exited ",bg[pid],pid);
			if(WEXITSTATUS(stat))
				fprintf(stderr,"with status=%d\n",WEXITSTATUS(stat));
			else
				fprintf(stderr,"normally\n");
			free(bg[pid]);
			bg[pid]=NULL;
		}
		printprompt(startwd);
		fflush(stdout);
	}


}

