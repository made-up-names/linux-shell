#define LIMIT  1024
#define LMT2 1024
#define PIDLIMIT 32768
int getprocessname(char* name,int pid,char* iname);
int launch(int argc,char* args[],int fg);
int printprompt(char * startwd);
int scaninput(char* input,int inputsize);
int inputsplit(char * input,char * commands[],char* delimiter,int* fg);
int parsecommand(char* command,char* args[],char * inputredir, char* outputredir,int* isappend);
int execute(int argc,char* args[],int fg);
int cd(int argc,char* args[]);
int pwd(char* args[]);
int echo(char* args[]);
int ls(int argc,char* args[]);
int lclock(int argc,char* args[]);
char startwd[LIMIT];
int pinfo(int argc,char* args[]);
char* bg[PIDLIMIT];
int jno[PIDLIMIT];
int njobs;
void exithandler(int sig);
int remindme(int argc,char* args[]);

