#include "printf_uart.h"

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILE_NO 0
#define STDOUT_FILE_NO 1
#define STDERR_FILE_NO 2

USART_TypeDef *G_uart;


void PrintfInit(USART_TypeDef *uarTx) {
    G_uart = uarTx;
    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}

void Uart1Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);    //使能USART1，GPIOA时钟

    //USART1_TX	 GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

    //USART1_RX		GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART1, ENABLE);                                        //使能串口1
}


int _isatty(int fd) {
    if (fd >= STDIN_FILE_NO && fd <= STDERR_FILE_NO)
        return 1;
    return 0;
}

int _write(int fd, char *ptr, int len) {
    if (fd == STDOUT_FILE_NO || fd == STDERR_FILE_NO) {
        uint32_t i;
        for (i = 0; i < len; i++) {
            USART_SendData(G_uart, ptr[i]);

            while (USART_GetFlagStatus(G_uart, USART_FLAG_TXE) == RESET);
        }
        while (USART_GetFlagStatus(G_uart, USART_FLAG_TC) == RESET);

        return len;

    }
    return -1;
}


int _close(int fd) {
    if (fd >= STDIN_FILE_NO && fd <= STDERR_FILE_NO)
        return 0;
    return -1;
}


int _lseek(int fd, int ptr, int dir) {
    (void) fd;
    (void) ptr;
    (void) dir;
    return -1;
}


int _read(int fd, char *ptr, int len) {
    if (fd == STDIN_FILE_NO) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET) {
        }
        *ptr = USART_ReceiveData(USART2);
        return 1;
    }
    return -1;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)
