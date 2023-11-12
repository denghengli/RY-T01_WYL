#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "systemdata.h"

/*�㲥��ַ*/
#define MODBUS_ADDR_BROAD       0xFF
/*Ĭ�ϵ�ַ*/
#define MODBUS_ADDR_DEFAULT     0X01

/* define Modbus Function Code */
#define MODBUS_COIL_REG_RD			    0x1	/* ����Ȧ�Ĵ��� */
#define MODBUS_DISIN_REG_RD			    0x2	/* ����ɢ����Ĵ��� */
#define MODBUS_HOLD_REG_RD	            0x3	/* �����ּĴ��� */
#define MODBUS_IPUT_REG_RD				0x4	/* ������Ĵ���*/
#define MODBUS_SIMCOIL_REG_WR	        0x5	/* д������Ȧ�Ĵ��� */
#define MODBUS_MULCOIL_REG_WR   	    0x0F/* д�����Ȧ�Ĵ���*/
#define MODBUS_SIMHOLD_REG_WR		    0x6	/* д�������ּĴ��� */
#define MODBUS_MULHOLD_REG_WR      	    0x10/* д������ּĴ��� */

/* RTU Ӧ����� */
#define RSP_OK				  0x00	/* �ɹ� */
#define RSP_ERR_CMD			  0x01	/* ��������� */
#define RSP_ERR_REG_ADDR	  0x02	/* �Ĵ�����ַ���� */
#define RSP_ERR_VALUE		  0x03	/* ����ֵ����� */
#define RSP_ERR_WRITE		  0x04	/* ��վ�豸����*/

#define MODBUS_RECV_ELN_MAX   UART_RXBUF_SIZE
#define MODBUS_SEND_ELN_MAX   UART_TXBUF_SIZE

/*�ӻ��Ĵ�������*/
#define SLAVE_HOLDREG_NUM	  HOLD_REGNUM
#define SLAVE_IPUTREG_NUM     INPUT_REGNUM
#define SLAVE_COILREG_NUM     1

/*�����Ĵ�������*/
#define MASTER_HOLDREG_NUM	  1
#define MASTER_IPUTREG_NUM    1
#define MASTER_COILREG_NUM    1

typedef enum
{
    EMB_HOLE=1,
    EMB_IPUT,    
    EMB_COIL,
    
    EMB_ALL
    
}MODBUS_REG_TYPE;


extern uint16_t ModsCRC16(const uint8_t *_cPtr,uint16_t _shLen);
extern uint16_t Mods_ULUH_TO_U16(uint8_t *_cptr);
extern uint16_t Mods_UHUL_TO_U16(uint8_t *_cptr);
extern uint16_t SlaveId_Get(void);
extern uint16_t MasterId_Get(void);

/*�ӻ�ͨѶ*/
extern uint8_t Slave_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat);/*���ڽ���MODBUS��������*/
extern uint8_t Slave_Pack_Analysis(UART_TRANS_T* _ptTrans);/*���ڽ���ͨ��DMA��ʽ���յ�����MODBUS����*/
extern uint8_t Slave_Recv_Proc(UART_TRANS_T* _ptTrans);/*���ڴ���ͨ��Slave_IsRecvDoneУ�������ȷ��MODBUS����*/
extern void Slave_Read_Reg (MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data);
extern void Slave_Write_Reg(MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data);

/*����ͨѶ*/
extern uint8_t Master_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat);/*���ڽ���MODBUS��Ӧ����*/
extern uint8_t Master_Pack_Analysis(UART_TRANS_T* _ptTrans);/*���ڽ���ͨ��DMA��ʽ���յ�����MODBUSӦ��*/
extern uint8_t Master_Recv_Proc(UART_TRANS_T* _ptTrans);/*���ڴ���ͨ��Master_IsRecvDoneУ�������ȷ��MODBUSӦ��*/
extern void Master_Read_Reg (MODBUS_REG_TYPE type,uint8_t mod,uint32_t addr,uint32_t nums,uint8_t* data);
extern void Master_Write_Reg(MODBUS_REG_TYPE type,uint8_t mod,uint32_t addr,uint32_t nums,uint8_t* data);

#endif


