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
int scaninput(char* input,int inputsize)
{
	for(int i=0;i<inputsize;i++)
	{
		scanf("%c",&input[i]);
		if(input[0]==EOF)
		{
			break;
		}
		if(input[i]=='\n'||input[i]==EOF)
		{
			input[i]='\0';
			break;
		}
	}		
	return 0;
}

