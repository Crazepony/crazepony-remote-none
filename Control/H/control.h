#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "stm32f10x.h"

void LoadRCdata(void);
void controlClibra(void);
void RockerUnlockcrazepony(void);
void KeyLockcrazepony(void);
void IMUcalibrate(void);
void Remotecalibrate(void);


extern int Throttle;
extern int Roll;
extern int Pitch;
extern int Yaw;
extern int8_t ClibraFlag;


#endif


