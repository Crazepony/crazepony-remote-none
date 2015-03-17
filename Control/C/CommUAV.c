

#include "CommUAV.h"
#include <stdio.h>
#include "stdint.h"
#include "NRF24L01.h"
#include "control.h"

//4rc定时50Hz发送
//其余指令触发发送
uint8_t sendBuf[32];
uint8_t sendCnt=0;
uint8_t checksum=0;

// 通信协议层与通信BUS层的接口，移植需要修改此函数。
#define NRFUpload() NRF24L01_TxPacket(sendBuf)

//对外的接口变量,输入rc数据
enum{THROTTLE,YAW,PITCH,ROLL};
uint16_t rcData[4]={1500,1500,1500,1500};             //1000-2000
//对外接口函数在.h


// #define CONSTRAIN(_x,min,max)  {if(_x<min) _x=min; if(_x > max) _x=max;}
// //  
// static float dbScaleLinear(float x, float x_end, float deadband)
// {
// 	if (x > deadband) {
// 		return (x - deadband) / (x_end - deadband);

// 	} else if (x < -deadband) {
// 		return (x + deadband) / (x_end - deadband);

// 	} else {
// 		return 0.0f;
// 	}
// }

static  void uart8chk(uint8_t _x) 
{
  sendBuf[sendCnt++]=_x;    
  checksum ^= _x; 
}
//大端
static void uart16chk(int16_t a)
{
    uart8chk((uint8_t)(a&0xff));
    uart8chk((uint8_t)(a>>8));
    
}


//
void CommUAVUpload(uint8_t cmd)
{
//  uint8_t len;
    
    sendCnt=0;
    
    uart8chk('$');
    uart8chk('M');
    uart8chk('<');
    checksum = 0;   
    
    switch(cmd)
    {
      case MSP_SET_4CON:
        uart8chk(8);            //data payload len
        uart8chk(cmd);
        uart16chk(Throttle);
        uart16chk(Yaw);
        uart16chk(Pitch);
        uart16chk(Roll);
        break;
      case MSP_ARM_IT:
        uart8chk(0);
        uart8chk(cmd);
        break;
      case MSP_DISARM_IT:
        uart8chk(0);
        uart8chk(cmd);
        break;
      case MSP_HOLD_ALT:
        uart8chk(0);
        uart8chk(cmd);
        break;
      case MSP_STOP_HOLD_ALT:
        uart8chk(0);
        uart8chk(cmd);
        break;
      case MSP_HEAD_FREE:
        uart8chk(0);
        uart8chk(cmd);
        break;
      case MSP_STOP_HEAD_FREE:
        uart8chk(0);
        uart8chk(cmd); 
        break;
      case MSP_AUTO_LAND_DISARM:
        uart8chk(0);
        uart8chk(cmd); 
        break;
			case MSP_ACC_CALI:
        uart8chk(0);
        uart8chk(cmd); 
        break;
			
			
			
    }
    uart8chk(checksum); 	  		
    
    //上传
    NRFUpload();
}

