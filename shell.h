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
int pinfo(int argc,char* args[]);
int remindme(int argc,char* args[]);

void exithandler(int sig);
void ctrlchandler(int sig);
void ctrlzhandler(int sig);


extern char startwd[LIMIT];
extern char* bg[PIDLIMIT];
extern int jno[PIDLIMIT];
extern int njobs;
extern int fore;


int njobs;
int fore;
int jno[PIDLIMIT];
char* bg[PIDLIMIT];
char startwd[LIMIT];
