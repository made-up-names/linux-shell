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
int lclock(int argc,char* args[])
{
	if(argc!=5)
	{
		printf("usage : clock -t interval -n stopduration\n");
		return 1;
	}
	int repeat=-1;
	int stop=-1,i=1;
	while(args[i]!=NULL)
	{
		if(i&1 && args[i][0]!='-')
			break;
		if(i&1==0)
		{
			int flag=0;
			for(int j=0;j<10;j++)
			{
				if(j!=atoi(args[i]))
					flag=1;
			}
			if(flag)
				break;
		}
		if(args[i-1][1]=='t')
			repeat=atoi(args[i]);
		else if(args[i-1][1]=='n')
			stop=atoi(args[i]);
		i++;

	}
	if(repeat<0 || stop<0)
	{
		printf("usage : clock -t interval -n stopduration\n");
		return 1;
	}
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	printf("%s\n",asctime(timeinfo));
	stop-=repeat;
	while(stop/repeat)
	{
		stop-=repeat;
		sleep(repeat);
		printf("%s\n",asctime(timeinfo));
	}

}

