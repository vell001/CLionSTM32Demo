#ifndef _PRINT_UART_H__

#define _PRINT_UART_H__

#include "stm32f10x.h"
#include <stdio.h>

void Uart1Init(u32 bound);

void PrintfInit(USART_TypeDef *uarTx);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

#endif //#ifndef _PRINT_UART_H__
