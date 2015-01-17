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
*/
/* Control.c file
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
------------------------------------
*/

#include "adc.h"
#include "control.h"
#include "ConfigTable.h"
#include "Led.h"
#include "NRF24L01.h"
#include "delay.h"

int Throttle;
int Roll;
int Pitch;
int Yaw;

uint16_t Throttle_Calibra=0;
uint16_t Pitch_Calibra   =0;
uint16_t Roll_Calibra    =0;
uint16_t Yaw_Calibra     =0;//摇杆校准值

char Lockflag      = 0;
char IMUcalibratflag = 0;


/*
摇杆数据采集
输入参数为美国手和日本手
*/
void LoadRCdata(unsigned char RomoteMode)
{
  switch(RomoteMode)
  {
    /*以下为美国手的对应关系*/
 case America: 
      Throttle=1500 - (Throttle_Calibra-(1000 + (1000 - (1000*Get_Adc_Average(3,15))/4096)));//采集油门摇杆的位置，由于硬件原因，需要用100-采集值
      Throttle=(Throttle<=1000)?1000:Throttle;               //越界判断
      Throttle=(Throttle>=2000)?2000:Throttle;               //越界判断

      Pitch= 1500 - (Pitch_Calibra - (1000 + (1000*Get_Adc_Average(1,15))/4096));//采集俯仰摇杆的位置，赋值给对应的俯仰变量
      Pitch=(Pitch<=1000)?1000:Pitch;                 //越界判断
      Pitch=(Pitch>=2000)?2000:Pitch;               //越界判断

      Roll= 1500 - (Roll_Calibra - (1000 + (1000*Get_Adc_Average(0,15))/4096));//采集横滚摇杆位置，赋值给对应的横滚变量
      Roll=(Roll<=1000)?1000:Roll;                //越界判断
      Roll=(Roll>=2000)?2000:Roll;              //越界判断

      Yaw= 1500 - (Yaw_Calibra - (1000 + (1000*Get_Adc_Average(2,15))/4096));//采集横滚摇杆位置，赋值给对应的偏航角
      Yaw=(Yaw<=1000)?1000:Yaw;                //越界判断
      Yaw=(Yaw>=2000)?2000:Yaw;              //越界判断
          break;
    /*以下为日本手的对应关系*/
 case Japan:
	 
      Throttle=1500 - (Throttle_Calibra-(1000 + (1000 - (1000*Get_Adc_Average(1,15))/4096)));//采集油门摇杆的位置，由于硬件原因，需要用100-采集值
      Throttle=(Throttle<=1000)?1000:Throttle;               //越界判断
      Throttle=(Throttle>=2000)?2000:Throttle;               //越界判断

      Pitch= 1500 - (Pitch_Calibra - (1000 + (1000*Get_Adc_Average(3,15))/4096));//采集俯仰摇杆的位置，赋值给对应的俯仰变量
      Pitch=(Pitch<=1000)?1000:Pitch;                 //越界判断
      Pitch=(Pitch>=2000)?2000:Pitch;               //越界判断

      Roll= 1500 - (Roll_Calibra - (1000 + (1000*Get_Adc_Average(0,15))/4096));//采集横滚摇杆位置，赋值给对应的横滚变量
      Roll=(Roll<=1000)?1000:Roll;                //越界判断
      Roll=(Roll>=2000)?2000:Roll;              //越界判断

      Yaw= 1500 - (Yaw_Calibra - (1000 + (1000*Get_Adc_Average(2,15))/4096));//采集横滚摇杆位置，赋值给对应的偏航角
      Yaw=(Yaw<=1000)?1000:Yaw;                //越界判断
      Yaw=(Yaw>=2000)?2000:Yaw;              //越界判断
      break;
  }
}


int8_t ClibraFlag;


void controlClibra(void)
{
	static u8 i;
	uint16_t sum[4]={0,0,0,0};
	static int8_t lednum=1;
  static int8_t clibrasumNum = 20;

	if((ClibraFlag == FAIL))//校准失败
	{

		for(i=0;i<clibrasumNum;i++)
		{
			
			sum[0] += 1000 + (1000 - (1000*Get_Adc_Average(3,15))/4096);
			sum[1] += 1000 + (1000*Get_Adc_Average(1,15))/4096;
			sum[2] += 1000 + (1000*Get_Adc_Average(0,15))/4096;
			sum[3] += 1000 + (1000*Get_Adc_Average(2,15))/4096;
			delay_ms(100);
			
			if(++lednum == led5 + 2)lednum = 2;
			LedSet(lednum - 1,0);
			LedSet(lednum ,1);
		}
		
		Throttle_Calibra = sum[0]/i;
		Pitch_Calibra    = sum[1]/i;
		Roll_Calibra     = sum[2]/i;
		Yaw_Calibra      = sum[3]/i;

		LoadRCdata(America);               //摇杆赋值
		if((Throttle>=1520)||(Throttle<1480)||(Pitch>=1520)||(Pitch<=1480)||(Roll>=1520)||(Roll<=1480)||(Yaw>=1520)||(Yaw<=1480))
						ClibraFlag       = FAIL;//校准失败
		else 		ClibraFlag       = OK;//校准成功标志
		
					
		
		SaveParamsToEEPROM();
		LedSet(led2,0);LedSet(led3,0);LedSet(led4,0);LedSet(led5,0);
	 }	
	
}




void UnlockCrazepony(void)
{
		while((Throttle>1050)||(Roll>1050))//解锁条件：油门拉到最低，方向打到最左解锁
	 {
		 LoadRCdata(America);               //摇杆赋值
		 NRF24L01_TxPacket(TxBuf);//9ms
	 }
}
