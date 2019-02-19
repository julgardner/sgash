//sgash - the Simple GArdner SHell
//Julian Gardner
//102215161
//CSCI 3453 001
//Last modified 2018-09-17

//sgash is a simple shell program.
//It waits for input, and executes a program depending on that input.
//It also will display error messages if the program cannot be found
//or has errors.
//sgash isolates itself from program failures by creating a child process
//to execute the programs the user specifies.

//Include statements
#include <stdio.h> //printf, scanf
#include <unistd.h> //fork, exec families
#include <sys/types.h> //pid_t
#include <sys/wait.h> //wait function
#include <string.h> //strtok, strcmp
#include <stdlib.h> //EXIT_FAILURE

//Constants
//max chars allowed in input array
const int MAX_CHARS_IN_COMMAND = 1023;
//max arguments allowed in input array, including the program to be executed
const int MAX_ARGS_IN_COMMAND = 128;

//Prototypes
//Fills an array with zero for next use.
void initArr(char *command, int length);
//Gets next command from stdin.
void getCommand(char *command, int maxChars);
//Parses a command into program and arguments.
int parseCommand(char *argumentVector[MAX_ARGS_IN_COMMAND], char *command,
  int maxArgs);
//Handle execution of the program.
int executeProgram(char *argumentVector[MAX_ARGS_IN_COMMAND], char *filename);


int main()
{
  //Declare variables
  int isExit = 0; //Has the user done the exit command? If so, nonzero.
  char command[MAX_CHARS_IN_COMMAND]; //Will be filled with user input
  char *args[MAX_ARGS_IN_COMMAND]; //argument vector

  //Main loop
  while (!isExit)
  {
    //Fill all arrays with zero
    for (int i = 0; i < MAX_CHARS_IN_COMMAND; ++i)
    {
      command[i] = 0;
    }
    for (int i = 0; i < MAX_ARGS_IN_COMMAND; ++i)
    {
      args[i] = NULL;
    }

    //Prompt for and get command from user
    printf("sgash> ");
    getCommand(command, MAX_CHARS_IN_COMMAND);

    //Parse command into program and arguments
    isExit = parseCommand(args, command, MAX_ARGS_IN_COMMAND);

    if (!isExit)
    {
      executeProgram(args, args[0]);
    }
  }
  printf("Exiting. Thank you for using sgash.\n");
  return 0;
}

//Gets next command from stdin.
//command must point to an array of null characters.
//maxChars must be the maximum amount of characters that
//may be read in from this function.
//Postcondition: command is filled with user input, including the newline at
//the end.
void getCommand(char *command, int maxChars)
{
  //Get the command via fgets
  fgets(command, maxChars, stdin);
  return;
}

//Parses a command into program and arguments.
//argumentVector must point to an array of arrays of chars.
//command must point to a array of chars
//with a newline and null char at the end.
//maxArgs must be the maximum number of allowed arguments,
//incuding the ending NULL pointer,
//and maxChars must be the maximum number of allowed chars in an argument.
//Postcondition: argumentVector is an argument vector compatible with the exec
//family of functions. If argumentVector[0] == "exit", returns 1.
//Idea to use strtok taken from this page:
//https://www-s.acm.illinois.edu/webmonkeys/book/c_guide/2.14.html#strtok
int parseCommand(char *argumentVector[MAX_ARGS_IN_COMMAND], char *command,
  int maxArgs)
{
  //break command into "words", replacing spaces and newlines with null char
  //argumentVector[0] points to the first word after this
  argumentVector[0] = strtok(command, " \n");
  int loopIndex = 1;
  do
  {
    //fill the rest of argumentVector with pointers to the next word
    argumentVector[loopIndex] = strtok(NULL, " \n");
    ++loopIndex;
  } while((loopIndex < maxArgs) && (argumentVector[loopIndex - 1] != NULL));

  //did the user type the exit command?
  if (!strcmp("exit", argumentVector[0]))
  {
    //string is equal to "exit"
    return 1;
  }
  else
  {
    //string is not equal to "exit"
    return 0;
  }
}

//Handle execution of the program via fork-exec-wait.
//argumentVector must be an array of arguments, with a NULL after the last one.
//filename should be a Linux complete file path,
//or a file to execute in the PATH enrironment variable's directory.
int executeProgram(char *argumentVector[MAX_ARGS_IN_COMMAND], char *filename)
{
  pid_t pid = fork();
  if (pid < 0)
  {
    //Process creation failed
    printf("Process Creation Error");
    return -1;
  }
  else if (pid == 0)
  {
    //Child process
    execvp(filename, argumentVector);
    //If execution reaches here, there was an error.
    perror(filename); //Print error message
    exit(EXIT_FAILURE); //avoid fork bomb
  }
  else
  {
    //Parent process
    wait(NULL);
  }
  return 0;
}
