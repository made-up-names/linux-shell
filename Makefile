shell: main.o execute.o inputsplit.o lclock.o parsecommand.o printprompt.o scaninput.o 	builtin.o exithandler.o launch.o ls.o pinfo.o remindme.o
	$(CC) -g -o shell main.o execute.o inputsplit.o lclock.o parsecommand.o printprompt.o scaninput.o  builtin.o exithandler.o launch.o ls.o pinfo.o remindme.o


clean:
	@rm -f *.o shell

main.o: shell.h main.c
	$(CC) -g -c main.c

execute.o: shell.h execute.c
	$(CC) -g -c execute.c
inputsplit.o: shell.h inputsplit.c
	$(CC) -g -c inputsplit.c
lclock.o: shell.h lclock.c
	$(CC) -g -c lclock.c
parsecommand.o: shell.h parsecommand.c
	$(CC) -g -c parsecommand.c
printprompt.o: shell.h printprompt.c
	$(CC) -g -c printprompt.c
scaninput.o: shell.h scaninput.c
	$(CC) -g -c scaninput.c
builtin.o: shell.h builtin.c
	$(CC) -g -c builtin.c
exithandler.o: shell.h exithandler.c
	$(CC) -g -c exithandler.c
launch.o: shell.h launch.c
	$(CC) -g -c launch.c
ls.o: shell.h ls.c
	$(CC) -g -c ls.c
pinfo.o: shell.h pinfo.c
	$(CC) -g -c pinfo.c
remindme.o: shell.h remindme.c
	$(CC) -g -c remindme.c

