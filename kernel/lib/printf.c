#include <bareio.h>
#include <barelib.h>

/*
 *  In this file, we will write a 'printf' function used for the rest of
 *  the semester.  In is recommended  you  try to make your  function as
 *  flexible as possible to allow you to implement  new features  in the
 *  future.
 *  Use "va_arg(ap, int)" to get the next argument in a variable length
 *  argument list.
 */

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
        // char buff[4]; // use a buffer to manage the ints
        uart_putc(((int) va_arg(ap,int)));
      }
      else if (*letter == 'x') {
        
      }
    }
    else {
      uart_putc(*letter);
    }
    counter++;
    letter = (char*) (format+counter);
  }

  // method that updates format 
  /*
  char* letter = (char*) format;
  while (*letter != '\0') {
    if (*letter == '%') {
      format = format + 1;
      letter = (char*) (format);
      if (*letter == 'd') {
        uart_putc('s');
      }
      else if (*letter == 'x') {
        
      }
    }
    else {
      uart_putc(*letter);
      
    }
    format = format + 1;
    letter = (char*) (format);
  }
  */

  
  va_end(ap);
}
