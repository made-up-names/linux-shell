#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//basically ikkada emuntundi ante
int printprompt(char * startwd);
int scaninput(char* input,int inputsize);
int parseinput(char * input,int inputsize);
int inputsplit(char * input,char * commands[],char* delimiter);
int parsecommand(char* command,char* args[]);
int execute(int argc,char* args[]);
int cd(int argc,char* args[],int mode);
char startwd[100000];
int main()
{
	//until exit command is typed
	//whileloop
	//print prompt
	//scan command
	//oarse command
	//execute command
	//commands to implement for now cd, ls,pwd.echo, system commands with and without arguments backgrounf and foreground, (pinfo),finished background processes
	// implement cd command now
	getcwd(startwd,500);
	int status=10;
	while(status--)
	{
		printprompt(startwd);
		char input[100000];
		scaninput(input,sizeof(input));		
	//	printf("%s\n",input);
		parseinput(input,sizeof(input));
	}
	return 0;
}
int printprompt(char * startwd)
{
	
	char buffer[100000];
	gethostname(buffer,500);
	char abcwd[100000];
	getcwd(abcwd,501);
	char cwd[100000];
	cwd[0]='~';
	cwd[1]='\0';
	int start=0,astart=0;
	printf("abcwd=%s,startwd=%s\n",abcwd,startwd);
	for(start=0;startwd[start]!='\0';start++);
	for(astart=0;abcwd[astart]!='\0';astart++);
	if(astart<start)
	{
		printf("<%s@%s:%s>",getlogin(),buffer,abcwd);
		return 0;
	}
	for(int i=start;abcwd[i]!='\0';i++)
	{
		cwd[i-start+1]=abcwd[i];
		cwd[i-start+2]='\0';
		printf("%s\n",cwd);
	}
	printf("<%s@%s:%s>",getlogin(),buffer,cwd);
	return 0;
}
int scaninput(char* input,int inputsize)
{
	//scan characters until \n or end of file
	int status=1;
	for(int i=0;i<inputsize;i++)
	{
		scanf("%c",&input[i]);
		if(input[i]=='\n'||input[i]==EOF)
		{
			input[i]='\0';
			break;
		}
	}		
	return 0;
}
int parseinput(char * input,int inputsize)
{
	//now check if single command or more than one command
	//input.split(;)
	char *commands[100];
	//for every malloc write an error statement
	int no=inputsplit(input,commands,";");
	printf("noof commands=%d\n",no);
	for(int i=0;i<no;i++)
	{
		char* args[1000];
		int noofargs=parsecommand(commands[i],args);
		printf("noofargs=%d\n",noofargs);
		printf("args=%s\n",args[1]);
		execute(noofargs,args);
	}
//	free(commands);
	return 0;
}
int inputsplit(char * input,char * commands[],char* delimiter)
{
	int i=0;
	commands[i]=strtok(input,delimiter);
	for(i=1;commands[i-1]!=NULL;i++)
	{
		commands[i]=strtok(NULL,delimiter);
//		printf("%s\n",delimiter);
	}
	commands[i-1]=NULL;
	return i-1;
}
int parsecommand(char* command,char* args[])
{
	char* delimiters=" \t\r\n\a\"\'";
	return inputsplit(command,args,delimiters);
}
int execute(int argc,char* args[])
{
	if(!args[0])
		return 0;
	//if args[0] in builtin commands
	char* builtincommands[3]={"cd","pwd","echo"};
	for(int i=0;i<sizeof(builtincommands)/sizeof(char*);i++)
	{
		if(!strcmp(args[0],builtincommands[i]))
		{
			int exitstatus=0;
			if(i==0)
			{

				int mode=0;
				if(argc!=1)
				{
					if(args[1][0]=='/' || strcmp(args[1],",")==1 || strcmp(args[1],"..")==1 )
						mode=1; // from dont append
					else if(args[1][0]=='~') 
						mode=2; // from ~ // append after to startwd
					else if(args[1][0]!='~' && args[1][0]!='/')
						mode=0; // mode 0 from current working directory append to cwd
				}
				// '.' '..' no argument 
				exitstatus=cd(argc,args,mode);
			}
			return exitstatus;
		}
	}
	return 0;
}
int cd(int argc,char* args[],int mode)
{
	char tomove[10000];
	if(argc>2)
	{
		printf("syntax error\n");
		return 0;
	}
	// relative path mode =0 
	//mode=1 absolute path from /home
	//mode=2 absolute path from ~

	// no directory
	if(argc==1)
	{
		int error=chdir(startwd);
		if(error<0)
			printf("Error occured\n");
		return 0;
	}

	char cwd[10000];
	getcwd(cwd,10000);
	
	if(mode==0)
	{
		strcpy(tomove,cwd);
		strcat(tomove,"/");
		strcat(tomove,args[1]);
	}
	else if(mode==2)
	{
		strcpy(tomove,startwd);
		if(args[1][1]!='\0')
			strcat(tomove,&args[1][1]);
	}
	else if(mode ==1 ) // everything else basically from /home/ 
	{
		
		strcpy(tomove,args[1]);
	}
	printf("%s\n",tomove);	
	int error=chdir(tomove);
	if(error<0)
		printf("Error: no such directory exists\n");
	return 0;
}
