#include <bareio.h>
#include <barelib.h>


/*
 * 'builtin_hello' prints "Hello, <text>!\n" where <text> is the contents 
 * following "builtin_hello " in the argument and returns 0.  
 * If no text exists, print and error and return 1 instead.
 */
byte builtin_hello(char* arg) {
  if (arg[6] == '\0') {
    printf("Error - bad argument\n");
    return 1;
  }
  else {
    printf("Hello,");
    printf(&(arg[5]));
    printf("!\n");
  }
  return 0;
  
  // const char* hello = "hello";
  // int index = 0;
  // while (arg[index] != '\0' && arg[index] == hello[index]) {
  //   index++;
  // }
  // if (arg[index] == '\0' && hello[index] == '\0') {
  //   printf("Error - bad argument\n");
  //   return 1;  
  // } 
  
  // else {
  //   int afterIndex = 0;
  //   char afterHello[1024];
  //   while (arg[index + afterIndex] != '\0') {
  //     afterHello[afterIndex] = arg[index + afterIndex];
  //     afterIndex++;
  //   }
  //   printf("Hello,");
  //   printf(afterHello);
  //   printf("!\n");
  //   return 0;  
  // }
}
