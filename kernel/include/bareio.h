/*
 *  This header includes function prototypes for IO procedures
 *  used by the UART and other print/scan related functions.
 */

char uart_putc(char);
char uart_getc(void);
void printf(const char*, ...);
int strcomp(const char *c1, const char *c2);

