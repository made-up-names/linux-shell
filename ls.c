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
int ls(int argc,char* args[])
{
	char cwd[LIMIT];
	getcwd(cwd,LIMIT-1);
	int l=0,a=0,file=0,error=0;
	//check if flags are there
	for(int i=1;args[i]!=NULL;i++)
	{
		if(args[i][0]=='-')
		{
			for(int j=1;args[i][j]!='\0';j++)
			{
				if(args[i][j]=='l')
					l=1;
				else if(args[i][j]=='a')
					a=1;
				else
					error=1;
			}
			if(args[i][1]=='\0')
				error=1;
		}
	}
	if(error==1)
	{
		printf("only ls -l and -a are supported\n");
		return 1;
	}
	for(int i=1;args[i]!=NULL;i++)
	{
		struct stat st;
		char path[LIMIT]="";
		if(args[i][0]!='-')
			file=1;
		else
		{
			continue;
		}
		int error=-1;
		if(args[i][0]=='~')
		{
			strcpy(path,startwd);
			strcat(path,&args[i][1]);
			error=stat(path,&st);
		}
		else if(args[i][0]=='/')
		{
			strcpy(path,args[i]);
			error=stat(path,&st);
		}
		else
		{
			//strcpy(path,cwd);
			//strcat(path,"/");
			strcat(path,args[i]);
			error=stat(path,&st);
		}
		if(error<0)
		{

			printf("%s\n",strerror(errno));
			continue;
		}
		//if a==1 and is dir file then print inodes they too
		if(S_ISDIR(st.st_mode))
		{
			//go to the directory and list all files in it  
			struct dirent * de;
			DIR *dr=opendir(path);
			if(dr==NULL)
			{
				printf("could not open the directory");
				continue;
			}
			printf("%s:\n",path);
			while((de=readdir(dr))!=NULL)
			{

				if(!(a==0 && de->d_name[0]=='.'))
				{
					struct stat st1;
					char file[100000];
					strcpy(file,path);
					strcat(file,"/");
					strcat(file,de->d_name);
					stat(file,&st1);
					if(l==1)
					{
						printf( (S_ISDIR(st1.st_mode)) ? "d" : "-");
						printf( (st1.st_mode & S_IRUSR) ? "r" : "-");
						printf( (st1.st_mode & S_IWUSR) ? "w" : "-");
						printf( (st1.st_mode & S_IXUSR) ? "x" : "-");
						printf( (st1.st_mode & S_IRGRP) ? "r" : "-");
						printf( (st1.st_mode & S_IWGRP) ? "w" : "-");
						printf( (st1.st_mode & S_IXGRP) ? "x" : "-");
						printf( (st1.st_mode & S_IROTH) ? "r" : "-");
						printf( (st1.st_mode & S_IWOTH) ? "w" : "-");
						printf( (st1.st_mode & S_IXOTH) ? "x" : "-");
						nlink_t nlink=st1.st_nlink;
						off_t st_size=st1.st_size;
						printf(" %lu",nlink);
						printf(" %s",getpwuid(st1.st_uid)->pw_name);
						printf(" %s",getgrgid(st1.st_gid)->gr_name);
						printf(" %ld",st_size);
						char mtime[80];
						time_t t=st1.st_mtime;
						struct tm lt;
						localtime_r(&t,&lt);
						strftime(mtime, sizeof( mtime), "%b %d %T", &lt);
						printf(" %s ", mtime);
					}
					printf("%s\n",de->d_name);
				}
			}
			closedir(dr);
		}
		else if(S_ISREG(st.st_mode))
		{
			if(l==1)
			{
				printf("-");
				printf( (st.st_mode & S_IRUSR) ? "r" : "-");
				printf( (st.st_mode & S_IWUSR) ? "w" : "-");
				printf( (st.st_mode & S_IXUSR) ? "x" : "-");
				printf( (st.st_mode & S_IRGRP) ? "r" : "-");
				printf( (st.st_mode & S_IWGRP) ? "w" : "-");
				printf( (st.st_mode & S_IXGRP) ? "x" : "-");
				printf( (st.st_mode & S_IROTH) ? "r" : "-");
				printf( (st.st_mode & S_IWOTH) ? "w" : "-");
				printf( (st.st_mode & S_IXOTH) ? "x" : "-");
				nlink_t nlink=st.st_nlink;
				off_t st_size=st.st_size;
				printf(" %lu",nlink);
				printf(" %s",getpwuid(st.st_uid)->pw_name);
				printf(" %s",getgrgid(st.st_gid)->gr_name);
				printf(" %ld",st_size);
				char mtime[80];
				time_t t=st.st_mtime;
				struct tm lt;
				localtime_r(&t,&lt);
				strftime(mtime, sizeof mtime, "%b %d %T", &lt);
				printf(" %s ", mtime);
			}
			printf("%s\n",path);
		}
		printf("\n");

	}
	if(file==0)
	{
		//printf("ls\n");
		struct dirent * de;
		DIR *dr=opendir(".");
		if(dr==NULL)
		{
			printf("could not open current directory\n");
			return 1;
		}
		while((de=readdir(dr))!=NULL)
		{
			if(!(a==0 && de->d_name[0]=='.'))
			{
				struct stat st1;
				stat(de->d_name,&st1);
				if(l==1)
				{
					printf( (S_ISDIR(st1.st_mode)) ? "d" : "-");
					printf( (st1.st_mode & S_IRUSR) ? "r" : "-");
					printf( (st1.st_mode & S_IWUSR) ? "w" : "-");
					printf( (st1.st_mode & S_IXUSR) ? "x" : "-");
					printf( (st1.st_mode & S_IRGRP) ? "r" : "-");
					printf( (st1.st_mode & S_IWGRP) ? "w" : "-");
					printf( (st1.st_mode & S_IXGRP) ? "x" : "-");
					printf( (st1.st_mode & S_IROTH) ? "r" : "-");
					printf( (st1.st_mode & S_IWOTH) ? "w" : "-");
					printf( (st1.st_mode & S_IXOTH) ? "x" : "-");
					nlink_t nlink=st1.st_nlink;
					off_t st_size=st1.st_size;
					printf(" %lu",nlink);
					printf(" %s",getpwuid(st1.st_uid)->pw_name);
					printf(" %s",getgrgid(st1.st_gid)->gr_name);
					printf(" %ld",st_size);
					char mtime[80];
					time_t t=st1.st_mtime;
					struct tm lt;
					localtime_r(&t,&lt);
					strftime(mtime, sizeof mtime, "%b %d %T", &lt);
					printf(" %s ", mtime);
				}
				printf("%s\n",de->d_name);
			}
		}
		closedir(dr);
		//file stat for cwd

	}
	return 0;
}

