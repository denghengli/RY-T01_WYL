#include "includes.h"

extern UART_TRANS_T     stUart1Trans;
extern UART_TRANS_T     stUart3Trans;

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    /**USART1 GPIO Configuration  
    PA9   ------> USART1_TX
    PA10   ------> USART1_RX 
    */
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    NVIC_EnableIRQ(USART1_IRQn);

    USART_InitStruct.BaudRate            = 115200;
    USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    LL_USART_Init(USART1, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USART1);

    LL_USART_Enable(USART1);
    LL_USART_DisableIT_CTS(USART1);
    LL_USART_ClearFlag_RXNE(USART1);
    LL_USART_EnableIT_RXNE(USART1);

}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    /**USART2 GPIO Configuration  
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX 
    */
    LL_USART_DeInit(USART2);
    
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    //  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    USART_InitStruct.BaudRate = 9600;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    LL_USART_Init(USART2, &USART_InitStruct);

    LL_USART_DisableIT_CTS(USART2);
    LL_USART_ConfigAsyncMode(USART2);
    LL_USART_Enable(USART2);
    LL_USART_ClearFlag_RXNE(USART2);
    LL_USART_EnableIT_RXNE(USART2);

}

/* USART3 init function */
void MX_USART3_UART_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    /**USART3 GPIO Configuration  
    PB10   ------> USART3_TX
    PB11   ------> USART3_RX 
    */
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    NVIC_EnableIRQ(USART3_IRQn);

    USART_InitStruct.BaudRate            = 9600;
    USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    LL_USART_Init(USART3, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USART3);

    LL_USART_Enable(USART3);
    LL_USART_DisableIT_CTS(USART3);
    LL_USART_ClearFlag_RXNE(USART3);
    LL_USART_EnableIT_RXNE(USART3);
}





/**********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
//int fputc(int ch, FILE *f)
//{
//	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
//	/* 写一个字节到USART2 */
//	LL_USART_TransmitData8(UARTPORT_DEBUG, (uint8_t) ch);
//	while (LL_USART_IsActiveFlag_TC(UARTPORT_DEBUG) == RESET);
//	LL_USART_ClearFlag_TC(UARTPORT_DEBUG);
//
//	return ch;
//
//}

/**********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
//int fgetc(FILE *f)
//{
//	/* 等待串口1输入数据 */
//	while (LL_USART_IsActiveFlag_RXNE(UARTPORT_DEBUG) == RESET);
//
//	return (int)LL_USART_ReceiveData8(UARTPORT_DEBUG);
//
//}

void Uart_Debug(char *s)
{
	while(*s != '\0')
	{
		LL_USART_TransmitData8(UARTPORT_DEBUG, (uint8_t) *s);
		while (LL_USART_IsActiveFlag_TXE(UARTPORT_DEBUG) == RESET)
		{
//			LL_USART_ClearFlag_TXE(UARTPORT_DEBUG);
		}
		
		s++;
	}	
}



/**********************************************************************************************************
*	函 数 名: Uart_SendData
*	功能说明: 通过串口将数据包发送出去
*	形    参: _Dat    数据指针
*             _ucLen  数据长度
*             USARTx  串口号
*	返 回 值: NONE
**********************************************************************************************************/
void Uart_SendData(USART_TypeDef *USARTx, const unsigned char *_Dat, unsigned char _ucLen)
{
	unsigned char i;
	
	for(i=0;i<_ucLen;i++)
	{
		LL_USART_TransmitData8(USARTx, _Dat[i]);//发送数据
		while(LL_USART_IsActiveFlag_TC(USARTx) == RESET);
		LL_USART_ClearFlag_TC(USARTx);
	}
}
