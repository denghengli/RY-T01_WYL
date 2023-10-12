#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "systemdata.h"

/*�㲥��ַ*/
#define MODBUS_ADDR_BROAD       0xFF
/*Ĭ�ϵ�ַ*/
#define MODBUS_ADDR_DEFAULT     0X01

/* define Modbus Function Code */
#define  	MODBUS_COIL_REG_RD			        0x1	/* ����Ȧ�Ĵ��� */
#define  	MODBUS_DISIN_REG_RD			        0x2	/* ����ɢ����Ĵ��� */
#define  	MODBUS_HOLDING_REG_RD	            0x3	/* �����ּĴ��� */
#define  	MODBUS_IN_REG_RD				    0x4	/* ������Ĵ���*/
#define  	MODBUS_SIMPCOIL_REG_WR	            0x5	/* д������Ȧ�Ĵ��� */
#define  	MODBUS_SIMPHOLDING_REG_WR		    0x6	/* д�������ּĴ��� */
#define  	MODBUS_MULHOLDING_REG_WR      	    0x10/* д������ּĴ��� */
#define  	MODBUS_MULCOIL_REG_WR   	        0x0F/* д�����Ȧ�Ĵ���*/

/* RTU Ӧ����� */
#define RSP_OK				  0x00	/* �ɹ� */
#define RSP_ERR_CMD			  0x01	/* ��������� */
#define RSP_ERR_REG_ADDR	  0x02	/* �Ĵ�����ַ���� */
#define RSP_ERR_VALUE		  0x03	/* ����ֵ����� */
#define RSP_ERR_WRITE		  0x04	/* ��վ�豸����*/

#define MODBUS_RECV_ELN_MAX   UART_RXBUF_SIZE
#define MODBUS_SEND_ELN_MAX   UART_TXBUF_SIZE

#define MODBUS_HOLDREG_LEN	  SYSTEM_DATA_LEN
#define MODBUS_INPUTREG_LEN   SYSTEM_DATA_LEN

extern unsigned char IsModsRecvDone(UART_TRANS_T* _ptTrans,unsigned char _cDat);/*���ڽ���MODBUS���ݰ�*/
extern void Mods_Recv_Analysis      (UART_TRANS_T* _ptTrans);/*���������н���modbus��������*/
extern void Read_Hold_Reg (unsigned int addr,unsigned int regs,unsigned char* data);
extern void Write_Hold_Reg(unsigned int addr,unsigned int regs,unsigned char* data);

#endif


