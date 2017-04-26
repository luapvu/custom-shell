//if ctrl c is received the program won't exit to the Ubuntu shell, it'll exit out of a current process i.e ping google.ca
void  ctrlC_handler(int c_signal) //terminates the process
{
     printf("\n"); //just print out a new empty line for user interface
}
