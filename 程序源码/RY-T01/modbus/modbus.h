#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "systemdata.h"

/*广播地址*/
#define MODBUS_ADDR_BROAD       0xFF
/*默认地址*/
#define MODBUS_ADDR_DEFAULT     0X01

/* define Modbus Function Code */
#define MODBUS_COIL_REG_RD			    0x1	/* 读线圈寄存器 */
#define MODBUS_DISIN_REG_RD			    0x2	/* 读离散输入寄存器 */
#define MODBUS_HOLD_REG_RD	            0x3	/* 读保持寄存器 */
#define MODBUS_IPUT_REG_RD				0x4	/* 读输入寄存器*/
#define MODBUS_SIMCOIL_REG_WR	        0x5	/* 写单个线圈寄存器 */
#define MODBUS_MULCOIL_REG_WR   	    0x0F/* 写多个线圈寄存器*/
#define MODBUS_SIMHOLD_REG_WR		    0x6	/* 写单个保持寄存器 */
#define MODBUS_MULHOLD_REG_WR      	    0x10/* 写多个保持寄存器 */

/* RTU 应答代码 */
#define RSP_OK				  0x00	/* 成功 */
#define RSP_ERR_CMD			  0x01	/* 功能码错误 */
#define RSP_ERR_REG_ADDR	  0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		  0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		  0x04	/* 从站设备故障*/

#define MODBUS_RECV_ELN_MAX   UART_RXBUF_SIZE
#define MODBUS_SEND_ELN_MAX   UART_TXBUF_SIZE

/*从机寄存器数量*/
#define SLAVE_HOLDREG_NUM	  HOLD_REGNUM
#define SLAVE_IPUTREG_NUM     INPUT_REGNUM
#define SLAVE_COILREG_NUM     1

/*主机寄存器数量*/
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

/*从机通讯*/
extern uint8_t Slave_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat);/*用于接收MODBUS请求数据*/
extern uint8_t Slave_Pack_Analysis(UART_TRANS_T* _ptTrans);/*用于解析通过DMA方式接收的整包MODBUS请求*/
extern uint8_t Slave_Recv_Proc(UART_TRANS_T* _ptTrans);/*用于处理通过Slave_IsRecvDone校验过后正确的MODBUS请求*/
extern void Slave_Read_Reg (MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data);
extern void Slave_Write_Reg(MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data);

/*主机通讯*/
extern uint8_t Master_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat);/*用于接收MODBUS响应数据*/
extern uint8_t Master_Pack_Analysis(UART_TRANS_T* _ptTrans);/*用于解析通过DMA方式接收的整包MODBUS应答*/
extern uint8_t Master_Recv_Proc(UART_TRANS_T* _ptTrans);/*用于处理通过Master_IsRecvDone校验过后正确的MODBUS应答*/
extern void Master_Read_Reg (MODBUS_REG_TYPE type,uint8_t mod,uint32_t addr,uint32_t nums,uint8_t* data);
extern void Master_Write_Reg(MODBUS_REG_TYPE type,uint8_t mod,uint32_t addr,uint32_t nums,uint8_t* data);

#endif


