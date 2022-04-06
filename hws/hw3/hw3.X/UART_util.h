#ifndef UART_UTIL__H__
#define UART_UTIL__H__

//This file is heavily influenced by Nick Marchuk's NU32.h from ME333

#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro

#define ME433_SYS_FREQ 48000000ul    // 80 million Hz

void UART_Startup();
void ME433_ReadUART1(char * string, int maxLength);
void ME433_WriteUART1(const char * string);

#endif // UART_UTIL__H__
