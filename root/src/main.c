#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "functions/structs.h"
#include "functions/signals.h"
#include "functions/parsers.h"
#include "functions/internalCommands.h"

// History structure, every session will keep track of last 10 commands and a counter of how many commands were executed
//index is used such that commands[10] is a circular array
typedef struct{
        char* commands[10];
        int counter;
        int index;
}History;


int main()

{
  //initialize command History and set counter and index to 0.
  History command_history;
  command_history.counter = 0;
  command_history.index = 0;
  signal(SIGINT, ctrlC_handler); //handler for ctrl + C signal. Anytime it's detected we will terminate the running process

  while(1){
          printf("-------Paul's Terminal-------: ");

          //Read in a command from command line(Assuming 4kb). Store the command into command history.
          char cmd[4096];
          fgets(cmd, 4096, stdin);

          if (feof(stdin)){ //if CTRL +D is pressed we stdin reads in an EOF, so we will want to close stdin pipe.
            printf("exit\n");
            exit(0);

          }

          command_history.index = command_history.index%10; // since our array is on the stack, we create a circular array to ensure we don't over index. Therefore we need the modulus of the counter
          command_history.commands[command_history.index] = strdup(cmd);
          command_history.index++; //increment index for next read in command
          command_history.counter++; // increment command counter to keep track of total commands

          Pipe_cmd cmd_parsed_by_pipes =  pipeParser(cmd);// Will parse the command into an array of program arguments and derrive the number of pipes used in the command.
          int numOfPipes = cmd_parsed_by_pipes.count; //holds the amount of '|' found in the command
          char ** programParamaters = cmd_parsed_by_pipes.program_Arguments; // holds a dynamic array from pipeParser function containing prog name and arguments. Needs to be freed later.
          int numOfPrograms = numOfPipes + 1; //amount of programs in a valid command is always 1 more than the number of pipes read in
          //at this point we either have an internal or external command. In the case of an internal command there should be NO pipes, so numOfPipes = 0. We parse the first command in our array at programParamaters[0] and check if it's an internal command


          int i = 0;
          int j =0;

          char ** arrayOfCommands[numOfPrograms];//parse each piped command into it's program name and arguments, store into array of commands
          for(i = 0; i < numOfPrograms; i++){
                arrayOfCommands[i] = programParser(programParamaters[i]);
          }

          //If it's an internal command, we execute internal system call, pwd, exit, cd, history
          //strcmp returns 0 if two strings are identical, I compare the parsed program Name to the internal commands exit, pwd, history and cd
          if(arrayOfCommands[0][0]==NULL){//if NULL is returned, user didn't input anything. The probably typed a return or enter.
                //do nothing, invalid command
          }else if(strcmp("exit", arrayOfCommands[0][0])==0){ //always compare first command's first element since it'll hold the program name
                exit(0);

          }else if(strcmp("pwd", arrayOfCommands[0][0])==0){
                char currDir[1024]; //string to hold current working directory
                getcwd(currDir, sizeof(currDir)); // system call to get current working directory and store into string
                printf("-------Paul's Terminal-------: %s\n", currDir);

          }else if(strcmp("history", arrayOfCommands[0][0])==0){
                internal_cmd_history(command_history.counter, command_history.commands); //prints out last 10 commands

          }else if(strcmp("cd", arrayOfCommands[0][0])==0){
                if(arrayOfCommands[0][1] != NULL){
                    internal_cmd_cd(arrayOfCommands[0][1]); //if it's a cd command, we know that there can only be 2 program arguments, cd being the first and the path existing in the second

                }else{
                    // user typed in cd with no paramaters...
                }

          }else{// If not a internal command we fork a child process
                if(numOfPipes >0){ // at least one pipe is read so we have at minimum 2 programs to execute

                      int arr_size = 2*numOfPipes; //for every pipe there is two ends for read and write
                      int pipe_array[arr_size]; //to create a pipe we must pass in an array of at least size 2. Here Arr_size can be larger than 2 in the case we need more than 1 pipe.
                      int arr_index = 0;
                      int b = 0;
                      for(b=0; b < numOfPipes; b++){
                      int *pointer = pipe_array +arr_index;
                      if(pipe(pipe_array + arr_index)){  //create pipes and file descriptors, since pipe_array is a pointer to the first element of the int array, we need to increment the pointer
                            printf("Error in pipe\n");
                            return -1;
                      }
                      arr_index = arr_index +2; //increment the index by 2 to set the next pipe. This is because each pipe takes up 1 read fd and 1 write fd
                    }
                    //sleep(30); // used to pause the code and ensure file descriptors and pipes are created. Check by running pidof my_shell then cd /proc/[pidof my_shell]/fdinfo

                    //With pipes & processes we will only have 3 CASES, a process with a single pipe connected to stdout, a process with 1 pipe connected to stdin or a process with 2 pipes connected at both ends
                    //For a given pipe command we will ALWAYS have CASE 1&2. CASE 3 only exists if we have more than one pipe commands
                    pid_t pid;
                    pid = fork();
                    if (pid < 0){
                          fprintf(stderr, "Fork Failed");
                          return 1;

                    }else if (pid ==0){ //CASE 1: Process is connected to a single pipe from the stdout end
                          dup2(pipe_array[1], STDOUT_FILENO); //since its connected to the stdout of the process we connect it to the write end of the first pipe
                          int k = 0;
                          for(k = 0; k < arr_size; k++){ // this loop closes all pipe ends since we have established our conection now.
                                if(k == 1){
                                      //we want to close all pipes execept for the one we currently use.
                                }else{
                                      close(pipe_array[k]);
                                }
                          }
                          execvp(arrayOfCommands[0][0], arrayOfCommands[0]); // The first child is created, it should execute the very first program listed in the pipe
                    }else{//ony parent process executes below code
                        //if there is more than one pipe we will have a CASE 2. We need a process connected to two pipes at the stdin and stdout end
                          int child_counter =1; //starts at one since we just forked a child. Keeps track of how many children we have
                          if(numOfPipes > 1){
                                //initializing read and write indexes for a  second process (child)
                                int a;
                                int prev_pipe_read = 0; // The second process get's to read from the previous pipe, which in this case is the first pipe in the array where the Read end is at index 0
                                int next_pipe_write = 3; // The second process will write out to the next pipe in the array which would have a write end of index 3

                                for(a = 1; a < numOfPipes; a++){ //loop through until we created all the processes necessary for CASE 2
                                      pid = fork();
                                      child_counter++; //everytime we fork another child is created, so let's keep track of this by incrementing child_counter
                                      if (pid < 0){
                                                fprintf(stderr, "Fork Failed");
                                                return 1;

                                      }else if (pid ==0){ // child
                                                dup2(pipe_array[prev_pipe_read], STDIN_FILENO); //connecting the read end of the previous pipe to stdin
                                                dup2(pipe_array[next_pipe_write], STDOUT_FILENO); //connecting the write end of the next pipe to stdout
                                                int k = 0;
                                                for(k = 0; k < arr_size; k++){ // this loop closes all pipe ends since we have established our conection now.
                                                      if(k == next_pipe_write || k == prev_pipe_read){
                                                            //we want to close all pipes execept for the one we currently use.
                                                      }else{
                                                            close(pipe_array[k]);

                                                      }
                                                }
                                                execvp(arrayOfCommands[a][0], arrayOfCommands[a]); //execute the next iteration of commands
                                      }else{
                                        prev_pipe_read = prev_pipe_read +2; //important to increment by two since we will be moving to the next set of pipes in the case we have another CASE 2 scenario
                                        next_pipe_write = next_pipe_write +2;
                                      }

                                  }
                          }

                          pid = fork(); //CASE 3: last child process to execute the very last program which will be connected to a single pipe
                          child_counter++; //everytime we fork another child is created, so let's keep track of this by incrementing child_counter
                          if (pid < 0){
                                fprintf(stderr, "Fork Failed");
                                return 1;

                          }else if (pid ==0){ // child
                                dup2(pipe_array[arr_size-2], STDIN_FILENO); //arr_size-2 is the last pipe's Read end, while arr_size -1 would be the last pipe's write end
                                int k = 0;
                                for(k = 0; k < arr_size; k++){ // this loop closes all pipe ends since we have established our conection now.
                                  if(k == arr_size-2){
                                        //we want to close all pipes execept for the one we currently use.
                                  }else{
                                        close(pipe_array[k]);
                                  }
                                }
                                execvp(arrayOfCommands[numOfPrograms-1][0], arrayOfCommands[numOfPrograms-1]); //execute the first command
                          }else{
                                  wait(NULL); //wait for last child to complete
                                  int k = 0;
                                  for(k = 0; k < arr_size; k++){
                                    close(pipe_array[k]); //as the parent, we close all the pipes
                                  }
                          }


                      }

                  }else{ // if no pipes are read in, then there is just one command to run with a set of arguments

                        pid_t pid;
                        pid = fork();
                        if (pid < 0){
                                fprintf(stderr, "Fork Failed");
                                return 1;

                        }
                        else if (pid ==0) { // child
                                sleep(0.5); //add some delay
                                execvp(arrayOfCommands[0][0], arrayOfCommands[0]); //since theres only one command it'll be the first element in the array

                        }else{
                                wait(NULL);
                        }

                  }

              }
              free(programParamaters); //deallocate all the heap memory we requested
              free(*arrayOfCommands);

              }
              return 0;

}
