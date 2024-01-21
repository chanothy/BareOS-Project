#include <bareio.h>
#include <barelib.h>
#include <shell.h>

#define PROMPT "bareOS$ "  /*  Prompt printed by the shell to the user  */
#define LINE_LEN 1024


/*
 * 'shell' loops forever, prompting the user for input, then calling a function based
 * on the text read in from the user.
 */

// int match(char *line, const char* cmp) {
//   int i =0;
//   for (; i< 1024 && cmp[i] == line[i] && cmp[i] != '\0'; i++);
//   return (cmp[i] == '\0');
// }

int wordCheck(char *line, const char* cmp) {
  int i = 0;
  while (i < 1024 && cmp[i] == line[i] && cmp[i] != '\0') {
    i++;
  }
  return (cmp[i] == '\0');
}

byte shell(char* arg) {
  while (1) {
    printf(PROMPT);
    char c;
    char line[LINE_LEN];
    for (int i = 0; i < LINE_LEN; ++i) line[i] = '\0';
    int i = 0;
    while (1) {
      c = uart_getc();
      if (c == '\n') {
        break;
      }
      line[i] = c;
      i++;
    }
    if (wordCheck(line,"hello")) {
      builtin_hello(line);
      // printf("hello_used\n");
    }
    else if (wordCheck(line,"echo")){
      builtin_echo(line);
      // printf("echo_used\n");
    }
    else {
      printf("Unknown command\n");
    }
  }

  return 0;
}

