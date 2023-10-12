#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "systemdata.h"

/*广播地址*/
#define MODBUS_ADDR_BROAD       0xFF
/*默认地址*/
#define MODBUS_ADDR_DEFAULT     0X01

/* define Modbus Function Code */
#define  	MODBUS_COIL_REG_RD			        0x1	/* 读线圈寄存器 */
#define  	MODBUS_DISIN_REG_RD			        0x2	/* 读离散输入寄存器 */
#define  	MODBUS_HOLDING_REG_RD	            0x3	/* 读保持寄存器 */
#define  	MODBUS_IN_REG_RD				    0x4	/* 读输入寄存器*/
#define  	MODBUS_SIMPCOIL_REG_WR	            0x5	/* 写单个线圈寄存器 */
#define  	MODBUS_SIMPHOLDING_REG_WR		    0x6	/* 写单个保持寄存器 */
#define  	MODBUS_MULHOLDING_REG_WR      	    0x10/* 写多个保持寄存器 */
#define  	MODBUS_MULCOIL_REG_WR   	        0x0F/* 写多个线圈寄存器*/

/* RTU 应答代码 */
#define RSP_OK				  0x00	/* 成功 */
#define RSP_ERR_CMD			  0x01	/* 功能码错误 */
#define RSP_ERR_REG_ADDR	  0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		  0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		  0x04	/* 从站设备故障*/

#define MODBUS_RECV_ELN_MAX   UART_RXBUF_SIZE
#define MODBUS_SEND_ELN_MAX   UART_TXBUF_SIZE

#define MODBUS_HOLDREG_LEN	  SYSTEM_DATA_LEN
#define MODBUS_INPUTREG_LEN   SYSTEM_DATA_LEN

extern unsigned char IsModsRecvDone(UART_TRANS_T* _ptTrans,unsigned char _cDat);/*用于接收MODBUS数据包*/
extern void Mods_Recv_Analysis      (UART_TRANS_T* _ptTrans);/*用于任务中进行modbus解析处理*/
extern void Read_Hold_Reg (unsigned int addr,unsigned int regs,unsigned char* data);
extern void Write_Hold_Reg(unsigned int addr,unsigned int regs,unsigned char* data);

#endif


