#ifndef __config_H
#define __config_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "Led.h"
#include "NRF24L01.h"
#include "Tim.h"
#include "sys_fun.h"
#include "SPI.h"
#include "control.h"
#include "stmflash.h"
#include "math.h"
#include "stdio.h"
#include "UART1.h"
#include "ConfigTable.h"
#include "adc.h"
#include "Key.h"
#include "CommUAV.h"
#endif


#define uart1baudSet 115200

#define AMERICAN_RC_MODE		//美国手（左手油门）或者日本手（右手油门）
#define UART_DEBUG					//是否输出打印信息以供调试

//------------------End of File----------------------------
