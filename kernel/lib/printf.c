#include <bareio.h>
#include <barelib.h>
#define buff 1024

/*
 *  In this file, we will write a 'printf' function used for the rest of
 *  the semester.  In is recommended  you  try to make your  function as
 *  flexible as possible to allow you to implement  new features  in the
 *  future.
 *  Use "va_arg(ap, int)" to get the next argument in a variable length
 *  argument list.
 */

void int_print(long num) {
  if (num == 0) {
    uart_putc('0');
    return;
  }

  // check for neg
  if (num < 0) {
    uart_putc('-');
    num = -num;
  }

  char buffer[buff]; 
  int index = 0;

  while (num > 0) {
    buffer[index] = '0' + (num % 10); // convert me ascii
    index++;
    num /= 10;
  }

  // go back and create the num
  for (int i = index - 1; i >= 0; i--) {
    uart_putc(buffer[i]);
  }
}

void hex_print(unsigned int num) {
  if (num == 0) {
    uart_putc('0');
    return;
  }

  char buffer[buff];
  int index = 0;

  if (num < 0) {
    num = -num;
  }

  while (num > 0) {
    int remainder =num % 16;
    if (remainder < 10) {
      // 0 ascii is 0
      buffer[index] = ('0' + remainder);
    }
    else {
      // a acii is 97
      buffer[index] = 'a'+ remainder - 10;
    }
    index++;
    num /= 16;
  }

  for (int i = index - 1; i >= 0; i--) {
    uart_putc(buffer[i]);
  }  
}

void str_print(char* str) {
  while (*str != '\0') {
    uart_putc(*str);
    str++;
  }
}

void printf(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  
  char* letter = (char*) format;
  int counter = 0;
  while (*letter != '\0') {
    if (*letter == '%') {
      counter++;
      letter = (char*) (format+counter);
      if (*letter == 'd') {
        int_print((long)va_arg(ap,int));
      }
      else if (*letter == 'x') {
        uart_putc('0');
        uart_putc('x');
        hex_print((unsigned int)va_arg(ap,int));
      }
      else if (*letter == 's') {
        str_print(va_arg(ap,char*));
      }
    }
    else {
      uart_putc(*letter);
    }
    counter++;
    letter = (char*) (format+counter);
  }
  
  va_end(ap);
}
