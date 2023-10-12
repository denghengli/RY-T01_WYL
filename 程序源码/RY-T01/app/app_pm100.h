#ifndef __APP_PM100_H__
#define __APP_PM100_H__

extern void SetPM100RcvNewFlg (unsigned char _cFlg);
extern unsigned char IsPM100RcvNew (void);
extern unsigned char IsPM100RcvDone(unsigned char _cDat);

extern void          PM100Ctl_Set       (unsigned char _cSw);

#endif



