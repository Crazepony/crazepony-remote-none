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

// uint32_t startTime[5],execTime[5];
// uint32_t realExecPrd[5];

#define debugprint


uint32_t t0,t1;

extern char Lockflag;
extern char IMUcalibratflag;
static char Locksta = 0xa5;


int main(void)
{
	static char ledsta;
	static u8 i;
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
	TIM4_Init(SysClock,TIME4_Preiod);	  //定时器4初始化，定时时间单位：(TIME4_Preiod)微秒

	
	
	LedSet(led2,1);
	LedSet(led3,1);
	
	
	LoadRCdata(America);               //摇杆赋值
	UnlockCrazepony();								 //摆杆启动
  Lockflag = 1;											 //解锁标志
	
  LedSet(led2,0);
	LedSet(led3,0);
	 
  while (1)             
	{ 
		
if(flag10Hz == 1)  //10Hz 
		{		
			flag10Hz = 0;
			ledsta = !ledsta;
			LedSet(signalLED,ledsta);				        
			
		 /*crazepony Lock*/
		  switch( Lockflag )
			{
				case 1:
					  if(Locksta == 0xa5) 
							{
								for(i=0;i<5;i++)         
								CommUAVUpload(MSP_ARM_IT);   //unlock Crazepony
								Locksta = 0x5a;
								Lockflag = 0;
								
							}
					  else if(Locksta == 0x5a )
							{
								for(i=0;i<5;i++)         
								CommUAVUpload(MSP_DISARM_IT);	//lock Crazepony
							  Locksta = 0xa5;
								Lockflag = 0;
							}
					break;
				case 0:
					if(Locksta == 0x5a)   LedSet(led5,1);
				  else if(Locksta == 0xa5) LedSet(led5,0);
					break;
		  }		
	
			/*IMUcalibratflag  */
      LedSet(led4,IMUcalibratflag);
			if(IMUcalibratflag) 
			{
				CommUAVUpload(MSP_ACC_CALI);
				IMUcalibratflag = 0;
			}
			
			
			/*remote calibrate*/
			if((ClibraFlag == FAIL)&&
				((Throttle<=1510)&&(Throttle>1490)&&
				(Pitch<=1510)&&(Pitch>=1490)&&
				(Roll<=1510)&&(Roll>=1490)&&
				(Yaw<=1510)&&(Yaw>=1490)))
				controlClibra();	  
			
      #ifdef debugprint
				printf("thr -->%d\r\n",Throttle);
				printf("pitch -->%d\r\n",Pitch);
				printf("roll -->%d\r\n",Roll);
				printf("yaw -->%d\r\n",Yaw);
				printf("-------------\r\n");
			#endif
			
		}

if(flag50Hz == 1)//
		{	
			t1 = micros() - t0; 
			t0 = micros();
			
			flag50Hz = 0;
			CommUAVUpload(MSP_SET_4CON);
			
		}
		
if(flag80Hz)// 80Hz 12.5ms
		{
			flag80Hz = 0;
			LoadRCdata(America);   
		}
		
		
	}
	
	
	
}

