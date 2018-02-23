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

int main()
{
	for(int i=0;i<PIDLIMIT;i++)
		bg[i]=NULL;
	getcwd(startwd,LIMIT); // ~ directory : startwd
	signal(SIGCHLD,exithandler); //exit handler for bg processes
	
	while(1)
	{
		printprompt(startwd);
		
		char input[LIMIT];
		scaninput(input,LIMIT);		
	
		//test scaninput 
		//printf("%s\n",input);
		
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
		//testinputsplit
		//for(int i=0;i<noofcommands;i++)
		//	printf("command= %s,fg=%d\n",commands[i],fg[i]);
		

		for(int i=0;i<noofcommands;i++)
		{
			char* args[LMT2]; 
			int noofargs=parsecommand(commands[i],args);
			execute(noofargs,args,fg[i]);
			for(int j=0;j<noofargs;j++)
			{
				if(args[j])
					free(args[j]);
				args[j]=NULL;
			}

		}
		for(int i=0;i<noofcommands;i++)
		{
			if(commands[i])
				free(commands[i]);
			commands[i]=NULL;
		}
	}
	return 0;
}


