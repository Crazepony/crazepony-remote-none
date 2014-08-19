#ifndef __config_H
#define __config_H


#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define   true 1
#define   false 0 
#define   bool  uint8_t


#include "stm32f10x.h"
#include "delay.h"
#include "Led.h"
#include "extern_variable.h"
#include "NRF24L01.h"
#include "Tim.h"
#include "sys_fun.h"
#include "SPI.h"
#include "control.h"
#include "stmflash.h"
#include "math.h"
#include "LCD5110.h"
#include "Display.h"
#include "adc.h"


#endif

//------------------End of File----------------------------
