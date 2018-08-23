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
int pinfo(int argc,char* args[])
{
	int i=1;
	int pid=0;
	if(args[i]==NULL)
		pid=getpid();

	while(args[i]!=NULL || pid!=0)
	{
		int j=0;
		if(pid==0)
			pid=atoi(args[i]);
		printf("pid---%d\n",pid);

		char proc[LIMIT]="/proc/";
		if(args[i]!=NULL)
			strcat(proc, args[i]);
		else
		{
			char str[100];
			sprintf(str,"%d",pid);
			strcat(proc,str);
		}
		char procstat[LIMIT]="";
		char procread[LIMIT];
		strcpy(procstat,proc);
		strcat(procstat,"/status");
		FILE *fd;
		fd=fopen(procstat,"r");
		if(!fd)
		{
			printf("no process exists\n");
			return 0;
		}
		for(int j=0;j<6;j++)
			fscanf(fd,"%s",procread);
		printf("Process status--%s\n",procread);
		for(int j=2;j<18;j++)
			fgets(procread,sizeof(procread),fd);
		char *memory=strtok(procread," ");
		memory=strtok(NULL," ");
		printf("%s{Virtual memory}\n",memory);
		fclose(fd);
		// read from procstat // 2 rows // 

		char exelink[LIMIT] = "";
		char exepath[LIMIT] = "";
		strcpy(exelink,proc);
		strcat(exelink, "/exe");
		int exepathsize = readlink(exelink, exepath, LIMIT - 1);
		exepath[exepathsize] = '\0';
		printf("Executable path--%s\n",exepath);
		pid=0;
		i++;
	}
	return 0;
}
