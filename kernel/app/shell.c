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

int intToChar(int num, char* storage) {
  if (num == 0) {
    storage[0] = '0';
    storage[1] = '\0';
    return 1;
  }
  int index = 0;
  while (num > 0) {
    storage[index] = '0' + (num % 10); // convert me ascii
    index++;
    num /= 10;
  }
  return index;
}

byte shell(char* arg) {
  int returnCode = 0;
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

    // check hello/echo commands
    if (wordCheck(buffer,"hello")) {
      returnCode = builtin_hello(buffer);
    }
    else if (wordCheck(buffer,"echo")){
      for (int j = 0; j < i; j++) {
        if (buffer[j] == '$' && buffer[j+1] == '?') { // j is used to check for echo, i is the total length of all the words
          char percentText[MAX];
          int length = intToChar(returnCode,percentText);
          if (length == 1) {
            buffer[j] = percentText[0];
            int next = j + 2;
            for (int k = j+1; k < i; k++) {
                buffer[k] = buffer[next];
                next++;
            }
          }
          else if (length == 2) {

          }
          else {

          }
          printf("{numberLen: %d}\n",length);
        }
      }
      // ---------------- buffer check
      printf("buffer: ");
      printf(buffer);
      printf("\n");
      // -----------------------------
      returnCode = builtin_echo(buffer);
    }
    else {
      printf("Unknown command\n");
    }

  }

  return 0;
}

