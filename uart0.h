//uart0.h
#ifndef _UART0_H_
#define _UART0_H_

void InitUART0 (void); /* Initialize Serial Interface       */
void UART0_Tx(char ch);
char UART0_Rx(void);
void UART0_Str(char *);
void UART0_Int(unsigned int);
void UART0_Float(float);

#endif