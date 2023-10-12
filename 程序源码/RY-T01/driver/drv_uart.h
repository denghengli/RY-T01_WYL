#ifndef _DRIVER_UART_
#define _DRIVER_UART_


#define  UARTPORT_COM        USART2  //����ͨѶʹ�ô���
#define  UARTPORT_DEBUG      USART1  //���Դ�ӡʹ�ô���

extern void Uart_SendData(USART_TypeDef *USARTx, const unsigned char *_Dat, unsigned char _ucLen);
extern void Uart_Debug(char *s);

extern void MX_USART1_UART_Init(void);
extern void MX_USART2_UART_Init(void);
extern void MX_USART3_UART_Init(void);


#endif


