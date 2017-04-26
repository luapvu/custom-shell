//A user's command is parsed into it's program and arguments
// i.e: " cat textfile | gzip -c | gunzip -c | tail -n 10"
// Arr.programAndArguments[0] = cat textfile
// Arr.programAndArguments[1] =  gzip -c
// Arr.programAndArguments[2] =  gunzip -c
// Arr.programAndArguments[3] =  tail -n 10
//count = 3 for 3 pipes

#ifndef PIPE_H
#define PIPE_H


typedef struct{
        char **program_Arguments;
        int count;
}Pipe_cmd;



#endif /* PIPE_H*/
