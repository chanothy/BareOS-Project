#include <bareio.h>
#include <barelib.h>

/*
 * 'builtin_echo' reads in a line of text from the UART
 * and prints it.  It will continue to do this until the
 * line read from the UART is empty (indicated with a \n
 * followed immediately by another \n).
 */

// gets all text after command and echos it
int getEchoText(void) {
  char buffer[1024];
  char c;
  int index = 0;
  while ((c = uart_getc()) != '\n') {
    buffer[index] = c;
    index++;
  }
  for (int i = 0; i < index; i++) {
    uart_putc(buffer[i]);
  }
  if (index != 0) {
    uart_putc('\n');
  }
  return index;
}

byte builtin_echo(char* arg) {
  int loopCount;
  int letterCount = 0;
  if (arg[4] == ' ') {
    printf(&(arg[5]));
    printf("\n");
    return 0;
  }
  else {
    // echo text returns index (amt of text)
    // while there is text keep incrementing
    loopCount = getEchoText();
    while (loopCount) {
      letterCount += loopCount;
      loopCount = getEchoText();
    }
  }
  return letterCount;
}
