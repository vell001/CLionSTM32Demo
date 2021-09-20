#include <stdio.h>
#include <stm32f10x_usart.h>
#include "main.h"
#include "printf_uart.h"


int main(void) {
    Uart1Init(115200); // 初始化Uart1

    PrintfInit(USART1); // printf 重定向到Uart1

    printf("hello world \r\n");
}
