#Variables
CC = gcc

#CFLAGS=-c -Wall
CFLAGS= -c

#This target compiles all files necessary for my terminal shell
all: compile
#Executes all object files created
#dependencies
#creates executable file my_shell
compile: main.o
		$(CC) main.o -o my_shell

main.o: main.c
	$(CC) $(CFLAGS) main.c

#removes my_shell executable files
clean:
	rm -rf *o my_shell
