#ifndef __APP_SPEEDCALCULATE_H__
#define __APP_SPEEDCALCULATE_H__

#define FLG_MEASOVER_DYNP   (0x01 << 0)  //ȫѹ�ɼ���ɱ�־
#define FLG_MEASOVER_STICP  (0x01 << 1)  //��ѹ�ɼ���ɱ�־
#define FLG_MEASOVER_TEMP   (0x01 << 2)  //�¶Ȳɼ���ɱ�־
#define FLG_MEASOVER         0X07        //���Ա�־����λ

extern unsigned char GetFlg_Measover(void);
extern void SetFlg_Measover(unsigned char _cflg);
extern void FlueGasSpeedCal_Proc(void);


#endif

