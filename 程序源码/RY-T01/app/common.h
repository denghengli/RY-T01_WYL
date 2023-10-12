#ifndef __COMMON_H__
#define __COMMON_H__

/*err code*/
#define SUCCESS  1
#define FAIL     0

#define IN
#define OUT

/*浮点数据小数位数*/
#define FLOAT_DECNUM 3

extern unsigned short UHUL_TO_U16(unsigned char *_cptr);
extern unsigned short ULUH_TO_U16(unsigned char *_cptr);

extern unsigned int   LEStructUint32Chg(unsigned int _idat);
extern unsigned short LEStructUint16Chg(unsigned short _shdat);
extern unsigned int   LEBufToUint32(unsigned char *_pBuf);
extern unsigned int   BEBufToUint32(unsigned char *_pBuf);
extern unsigned short LEBufToUint16(unsigned char *_pBuf);
extern unsigned short BEBufToUint16(unsigned char *_pBuf);
extern int  str_to_int(char *_pStr);
extern void int_to_str(int _iNumber, char *_pBuf, unsigned char _len);
extern void mem_set(char *_tar, char _data, int _len);
extern int  str_cmp(char * s1,  char * s2);
extern void str_cpy(char *_tar, char *_src);
extern int  str_len(char *_str);
	
extern void CoarseDelay(unsigned int _dly);
extern void Delay_us   (unsigned int _us);
extern void Delay_Ms   (unsigned int _ms);

extern void FloatLimit(float *fDat,unsigned char n);

								
/******************************调试打印开关************************************/		
																				
#define  DEBUG_TASK     0
#define  DEBUG_COM      0
#define  DEBUG_PARAMSET 0
#define  DEBUG_PT100    0
#define  DEBUG_ATPPRESS 0
#define  DEBUG_GASPRESS 0
#define  DEBUG_PM10     0	
							
#define  UART_DEG_EN    1  //调试 与参数设置 不能同时打开																			
        
#if UART_DEG_EN
#define UART_DEBUG(d,m)		do {if(d) printf m ;} while(0)//调试信息 
#else
#define UART_DEBUG(d,m)
#endif

/*线程安全的串口打印，使用互斥量进行保护*/
extern void LOG_PRINT(char n,char *format, ...);
	
#endif



