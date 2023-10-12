#ifndef __APP_SPEEDCALCULATE_H__
#define __APP_SPEEDCALCULATE_H__

#define FLG_MEASOVER_DYNP   (0x01 << 0)  //全压采集完成标志
#define FLG_MEASOVER_STICP  (0x01 << 1)  //静压采集完成标志
#define FLG_MEASOVER_TEMP   (0x01 << 2)  //温度采集完成标志
#define FLG_MEASOVER         0X07        //所以标志被置位

extern unsigned char GetFlg_Measover(void);
extern void SetFlg_Measover(unsigned char _cflg);
extern void FlueGasSpeedCal_Proc(void);


#endif

