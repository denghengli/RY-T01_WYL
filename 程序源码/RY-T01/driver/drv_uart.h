#ifndef _DRIVER_UART_
#define _DRIVER_UART_

#define UARTPORT_COM    USART1  //����ͨѶʹ�ô���
#define UARTPORT_DEBUG  USART2  //���Դ�ӡʹ�ô���

/*���ڲ�����*/
#define BAUD_RATE_2400   2400               
#define BAUD_RATE_4800   4800            
#define BAUD_RATE_9600   9600             
#define BAUD_RATE_19200  19200               
#define BAUD_RATE_38400  38400              
#define BAUD_RATE_57600  57600               
#define BAUD_RATE_115200 115200               
/*��������λ*/      
#define DATA_BITS_8      8
#define DATA_BITS_9      9
/*����ֹͣλ*/
#define STOP_BITS_1      1
#define STOP_BITS_2      2
/*������żУ��*/
#define PARITY_NONE      0
#define PARITY_ODD       1 //��У��
#define PARITY_EVEN      2 //żУ��

#define UART_CFGPARM_VALID(B,P,D,S)	((B==BAUD_RATE_2400 || B==BAUD_RATE_4800 || B==BAUD_RATE_9600 || B==BAUD_RATE_19200 || B==BAUD_RATE_38400 || B==BAUD_RATE_115200) && \
                                     (P==PARITY_NONE || P==PARITY_ODD || P==PARITY_EVEN) && \
									 (D==DATA_BITS_8 || D==DATA_BITS_9) && \
									 (S==STOP_BITS_1 || S==STOP_BITS_2))  

/*�������ò��� */
typedef struct
{
    unsigned int BaudRate;
    unsigned int Parity;
    unsigned int DataBits;
    unsigned int StopBits;
}UART_CONFIG;

extern void Uart_SendData(USART_TypeDef *USARTx, const unsigned char *_Dat, unsigned char _ucLen);
extern void Uart_Debug(char *s);

extern void MX_USART1_UART_Init(void);
extern void MX_USART2_UART_Init(void);
extern void MX_USART3_UART_Init(void);

extern void Modify_UART_BaudRate(USART_TypeDef *USARTx, UART_CONFIG *Config);

#endif


