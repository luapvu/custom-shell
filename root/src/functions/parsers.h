#include "structs.h"

//parses a string input into an array of arguments and identifies the program name
char** programParser(char input[]){

      char **pgmArguments = (char**)calloc(1, sizeof(char*)); //a pointer to a string array (which is also a pointer to characters)
      int paramater_counter = 0; //keeps track of how many white space separated arguments are enered into the cmd line

      char* pointer;
      pointer = strtok(input, (" \f\n\r\t\v")); //strtok returns a pointer to a string of the read in input up to the whitespace
      pgmArguments[paramater_counter] = pointer; //set the program name to the pointer since this is the first read in command before a white space. First paramater is always program name

      while(pointer!=NULL){ //if strtok doesn't return null, there is more input to read. It returns null after everything is read

            pgmArguments[paramater_counter] = pointer; //set the character array to the read in value
            pointer = strtok(NULL, " \f\n\r\t\v"); //again, we read in the next input
            paramater_counter++; //increment the paramater_counter should always be at least 1 even if NULL is read in after the second strtok since the 1 paramater is the prog name

            int size = (paramater_counter +1)*sizeof(char*); //we want size to allocate the arguments array some memory on the heap so we can return the read inputs, therefore we dynamically increase the size of the array after each read
            pgmArguments = (char**)realloc(pgmArguments, size);

      }

      //if we read in more than one argument we need to make sure there's no garbage character left after the strtok on the last  element before NULL was read
      if(paramater_counter > 1){ // meaning the program has arguments
            int last_char = strlen(pgmArguments[paramater_counter-1]); //-1 to take into account that we incremented argument_counter 1 too many times when it strtok a NULL
            pgmArguments[paramater_counter-1][last_char] = 0; //the last argument's last character string should be NULL, not a garbage character
      }
      pgmArguments[paramater_counter] = NULL; //setting the last element in the argument array to NULL for execvp to work correctly


      return pgmArguments; //note this is a dynamic array of charaters so we need to free this in main

}

//parses the user's command line input into an array of inputs and identifies the program name
Pipe_cmd pipeParser(char input[]){

      char **nameAndArguments = (char**)calloc(1, sizeof(char*)); //dynamic holding program name and arguments
      int pipe_counter = 0; //keeps track of how many '|' lines are read

      char* pointer;
      pointer = strtok(input, ("|")); //strtok returns a pointer to a string of the read in input up to the '|' character
      nameAndArguments[pipe_counter] = pointer; //will hold the string up until a | is found, if no pipe is found it will hold the entire command

      //at this point we either read in a pipe or didnt, regardless our nameAndArguments array will hold a a string and in the case there is another '|' we keep parsing until NULL
      while(pointer!=NULL){ //if strtok doesn't return null, there is more input to read. It returns null after everything is read

            nameAndArguments[pipe_counter] = pointer; //set the character array to the read in value
            pointer = strtok(NULL, "|"); //again, we read in the next input

            if(pointer !=NULL){ // pipe counter would be 0 in the case no pipes are found in the cmd, since the first strtok will contain the cmd string and the second strtok will = NULL
              pipe_counter++; //if pointer isn't NULL, then we read in more strings up until a |, that means there's more '|' in the cmd sso increment pipe counter
            }

            int size = (pipe_counter +1)*sizeof(char*); //we want size to allocate the arguments array some memory on the heap so we can return the read inputs, therefore we dynamically increase the size of the array after each read
            nameAndArguments = (char**)realloc(nameAndArguments, size);
      }
      int i;

      // nameAndArguments[pipe_counter]  will hold the last  string after a | or it will hold the entire cmd string if no pipe was read in
      Pipe_cmd parsedCMD;
      parsedCMD.program_Arguments = nameAndArguments; //note this is a dynamic array on the heap, need to free this in main
      parsedCMD.count = pipe_counter;
      return parsedCMD;

}
