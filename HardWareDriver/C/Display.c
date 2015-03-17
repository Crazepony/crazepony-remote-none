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
Tim.c file
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
1.这部分代码，是最初几个版本用显示各类参数时用的，那时用的是诺基亚5110的屏幕，这部分代码是可以用的，而且很好用
2.大家可以去研究，5110驱动代码在源文件里面，没有加入该工程而已，所以在左边的工程文件看不到它
------------------------------------
*/

/*****************************************************************
 * 文件名   ：Display.c
 * 描述     : 屏幕显示驱动
 * 实验平台 ：STM32f103T8U6 
 * 库版本   ：ST3.5.0
 * 作者     ：MJ
 * 时间     ：2014-2-18 0:47:33
 * 情况     ：可以使用
 * 其它     ：今天查到考研成绩，哎  含着悲愤写着程序
 ******************************************************************/

#include "adc.h"

    int Throttle;
    int Roll;
    int Pitch;
    int Yaw;
    int Battery;//定义待显示量


int Throttle_Offest=0;

int Pitch_Offest = -49;
int Roll_Offest  = -49;
int Yaw_Offest   = -48;//遥控微调

int Pitch_Mid =50;
int Roll_Mid  =50;
int Yaw_Mid   =50;//摇杆中位值

    
/*
摇杆数据采集
输入参数为美国手和日本手
*/
void GetAD(unsigned char RomoteMode)
{

  switch(RomoteMode)
  {
    /*以下为美国手的对应关系*/
 case America:
                
      Throttle=Throttle_Offest + 99- (100*Get_Adc_Average(3,15))/4096;//采集油门摇杆的位置，由于硬件原因，需要用100-采集值
      Throttle=(Throttle<=0)?0:Throttle;                 //越界判断
      Throttle=(Throttle>=99)?99:Throttle;               //越界判断

      Pitch=Pitch_Mid + Pitch_Offest + (100*Get_Adc_Average(1,15))/4096;//采集俯仰摇杆的位置，赋值给对应的俯仰变量
      Pitch=(Pitch<=0)?0:Pitch;                 //越界判断
      Pitch=(Pitch>=99)?99:Pitch;               //越界判断

      Roll=Roll_Mid + Roll_Offest + (100*Get_Adc_Average(0,15))/4096;//采集横滚摇杆位置，赋值给对应的横滚变量
      Roll=(Roll<=0)?0:Roll;                //越界判断
      Roll=(Roll>=99)?99:Roll;              //越界判断

      Yaw=Yaw_Mid + Yaw_Offest + (100*Get_Adc_Average(2,15))/4096;//采集横滚摇杆位置，赋值给对应的偏航角
      Yaw=(Yaw<=0)?0:Yaw;                //越界判断
      Yaw=(Yaw>=99)?99:Yaw;              //越界判断

//       Battery=(100*Get_Adc_Average(8,15))/2600;//采集遥控电池电压，赋值飞对应的电池变量
//       Battery=(Battery>=99)?99:Battery;               //越界判断
//    
          break;
    /*以下为日本手的对应关系*/
 case Japan:
      Throttle=Pitch_Mid + Throttle_Offest + (100*Get_Adc_Average(1,15))/4096;//采集油门摇杆的位置，由于硬件原因，需要用100-采集值
      Throttle=(Throttle<=0)?0:Throttle;                 //越界判断
      Throttle=(Throttle>=99)?99:Throttle;               //越界判断

      Pitch=Pitch_Mid + Pitch_Offest+ 99 -(100*Get_Adc_Average(3,15))/4096;//采集俯仰摇杆的位置，赋值给对应的俯仰变量
      Pitch=(Pitch<=0)?0:Pitch;                 //越界判断
      Pitch=(Pitch>=99)?99:Pitch;               //越界判断

      Roll=Roll_Mid + Roll_Offest + (100*Get_Adc_Average(0,15))/4096;//采集横滚摇杆位置，赋值给对应的横滚变量
      Roll=(Roll<=0)?0:Roll;                //越界判断
      Roll=(Roll>=99)?99:Roll;              //越界判断

      Yaw=Yaw_Mid + Yaw_Offest + (100*Get_Adc_Average(2,15))/4096;//采集横滚摇杆位置，赋值给对应的偏航角
      Yaw=(Yaw<=0)?0:Yaw;                //越界判断
      Yaw=(Yaw>=99)?99:Yaw;              //越界判断

//       Battery=(100*Get_Adc_Average(8,15))/2600;      //采集遥控电池电压，赋值飞对应的电池变量
//       Battery=(Battery>=99)?99:Battery;              //越界判断
//   
      break;
    
    
    
  }

  

}





