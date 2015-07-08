/*
      ____                      _____                  +---+
     / ___\                     / __ \                 | R |
    / /                        / /_/ /                 +---+
   / /   ________  ____  ___  / ____/___  ____  __   __
  / /  / ___/ __ `/_  / / _ \/ /   / __ \/ _  \/ /  / /
 / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /
 \___/_/   \__,_/ /___/\___/_/    \___ /_/ /_/____  /
                                                 / /
                                            ____/ /
                                           /_____/
main.c file
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
------------------------------------
*/
#include "config.h"             //包含所有的驱动头文件

extern char IMUcalibratflag;
extern char Lockflag;


int main(void)
{
	static char ledsta;
	/***********************************/
	SystemClock_HSI(9);           //系统时钟初始化，时钟源内部HSI
	cycleCounterInit();				    // Init cycle counter
	SysTick_Config(SystemCoreClock / 1000);	//SysTick开启系统tick定时器并初始化其中断，1ms
	UART1_init(SysClock,uart1baudSet); //串口1初始化
  NVIC_INIT();	                //中断初始化
  STMFLASH_Unlock();            //内部flash解锁
  LoadParamsFromEEPROM();				//加载系统参数配置表
  LedInit();	                  //IO初始化
  Adc_Init();										//摇杆AD初始化
	KeyInit();										//按键初始化
 	NRF24L01_INIT();              //NRF24L01初始化
  SetTX_Mode();                 //设无线模块为接收模式
  controlClibra();							//遥控摇杆校准
#ifdef UART_DEBUG  
	TIM3_Init(SysClock,2000);			//定时器初始化，1s为周期打印摇杆值
#endif
	TIM4_Init(SysClock,TIME4_Preiod);	  //定时器4初始化，定时时间单位：(TIME4_Preiod)微秒
	
	LedSet(led2,1);
	LedSet(led3,1);
	
	LoadRCdata();                //摇杆赋值
	//RockerUnlockcrazepony();	 //摆杆启动
  Lockflag = 0;								 //解锁标志，1表示产生了一次按键操作，0表示该按键操作已经发送到飞控
	
  LedSet(led2,0);
	LedSet(led3,0);
	 
  while (1)             
	{ 
		//10Hz loop
		if(flag10Hz == 1)  //10Hz 
		{		
			flag10Hz = 0;
			/*status led*/
			ledsta = !ledsta;
			LedSet(signalLED,ledsta);				        
			/*crazepony Lock*/
			KeyLockcrazepony();
			/*IMUcalibrate  */
			IMUcalibrate();
			/*remote calibrate*/
			Remotecalibrate();
		}

		//50Hz loop
		if(flag50Hz == 1)
		{
			LoadRCdata();
			flag50Hz = 0;
			
		}
		
		// 80Hz 12.5ms
		if(flag80Hz)
		{
			flag80Hz = 0;
			CommUAVUpload(MSP_SET_4CON);   
		}
	}
}

