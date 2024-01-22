#include <bareio.h>
#include <barelib.h>
#include <shell.h>

#define PROMPT "bareOS$ "  /*  Prompt printed by the shell to the user  */
#define MAX 1024


/*
 * 'shell' loops forever, prompting the user for input, then calling a function based
 * on the text read in from the user.
 */


// check if beginning contains command
int wordCheck(char *buffer, const char* cmp) {
  int i = 0;
  // breaks out if the beginning of buffer is not == to cmp and cmp wil not be at end
  while (i < 1024 && cmp[i] == buffer[i] && cmp[i] != '\0') {
    i++;
  }
  return (cmp[i] == '\0');
}

byte shell(char* arg) {

  while (1) {
    printf(PROMPT);
    char c;
    char buffer[MAX];

    //clears 
    for (int i = 0; i < MAX; ++i) {
      buffer[i] = '\0';
    }
    int i = 0;

    //check for enter
    while ((c = uart_getc()) != '\n') {
      buffer[i] = c;
      i++;
    }
    int returnCode;

    // check hello/echo commands
    if (wordCheck(buffer,"hello")) {
      returnCode = builtin_hello(buffer);
    }
    else if (wordCheck(buffer,"echo")){
      returnCode = builtin_echo(buffer);
    }
    else {
      printf("Unknown command\n");
    }

    printf("test: %d\n",returnCode);
  }

  return 0;
}

