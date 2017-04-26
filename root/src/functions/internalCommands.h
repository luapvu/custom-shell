
//This function takes in a command array and a counter of how many commands exist in total
//command array is of size 10 and only holds the most recent 10 commands
//this function will print out the last 10 commands executed
void internal_cmd_history(int counter, char* commands[]){

      int i = 0; //used to ensure we only ever print out a maximum of 10 command historys
      if (counter >10){ //if the user has used more than 10 commands during the session, we need to update i such that it is the 10th most recent
          i = counter -10;

      }

      for(i; i < counter; i++){ //loops to print out up to the last 10 commands used
          if(i >=10){
              printf(" [%d]: %s\n", i, commands[i%10]); //modulus 10 of i is past 10, for example 21st command exists inside a circular array structure of size 10,

          }else{ //user has used less than 10 commands during the session

              if(commands[i]!= NULL){ //if the value at this index does not hold a NULL value print out what's at the index
                printf(" [%d]: %s\n", i, commands[i]);

              }
          }
      }
}


void internal_cmd_cd(char *path){
      int length = strlen(path) +1; //however long this directory length is we add 1 to it for the null terminating string char
      char input_dir[length]; // creates a directory string for us to use in chdir
      int dir_response; //chdir will return an integer, store the response here
      char current_dir[1024];
      int i; //used in for loops

      //i will be 1 if we are cd into a forward folder, 0 if we are ../ into a previous directory
      //if the first to characters are ".." we set i to 0 in the for loop
      if(*(path) == '.' && *(path+1) == '.' && length > 2){

        //set i = 0, copy the entire string from the array into the input directory
        for( i =0; i < length; i++){
          input_dir[i] = *(path+i);
        }
        //since the input_dir was copied, we need to insert the \0 character to ensure chdir reads up to the last char
        input_dir[length] = '\0';
        dir_response = chdir(input_dir); //change to the specified directory


      }else{
            //need to insert "/" character as the first char for chdir
            input_dir[0] = '/';
            //copy the rest of the specified dir into input_dir
            for( i =1; i < length; i++){
              input_dir[i] = *(path+i-1); //we need to -1 since i starts at 1 since input_dir[0] holds '/'
            }
            input_dir[length] = '\0'; //ensures the last character in the array is null

            getcwd(current_dir, sizeof(current_dir)); //gets current working directory user is in
            char *complete_directory = strcat(current_dir, input_dir); //contcatonates the current directory and the specified directory
            dir_response = chdir(complete_directory);

      }
      if(dir_response != 0){ //if a successful directory change was executed we should get 0...if not print out error msg
        printf("-------Paul's Terminal-------: No such file or directory\n");
      }
      getcwd(current_dir, sizeof(current_dir)); //print out the current directory for user to see
      printf("-------Paul's Terminal-------: %s\n", current_dir);


}
