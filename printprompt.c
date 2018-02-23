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
int printprompt(char * startwd)
{
	// to print < username @hostname : current directory >~
	
	// buffer : hostname
	char buffer[LIMIT];
	gethostname(buffer,LIMIT-1);
	
	// abcwd: absolute path of current directory
	char abcwd[LIMIT];
	getcwd(abcwd,LIMIT-1);
	
	// username :pw-> pw_name // passwd file
	register uid_t uid = geteuid ();
	register struct passwd * pw= getpwuid (uid); //get username
	
	// if len of absolute path of cwd < len of startwd --> cwd is ancestor of startwd therefore
	// print absolute path
	if(strlen(abcwd)<strlen(startwd))
	{
		printf("<%s@%s:%s>",pw->pw_name,buffer,abcwd);
		return 0;
	}
	int start=strlen(startwd);
	printf("<%s@%s:~%s>",pw->pw_name,buffer,&abcwd[start]);
	return 0;
}

