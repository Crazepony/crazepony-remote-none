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
初版时间: 2014-03-28
功能：
1.遥控主函数入口
2.遥控方面，我没有花过多的时间去整理程序，只是规定了数据格式
3.大家可以做优化和修改~。~
------------------------------------
*/

#include      "Config.h"			 //头文件包含

#define  R_Mode  America  //修改宏定义，切换遥控操作方式为美国手还是日本手

extern int  Pitch_Offest;
extern int  Roll_Offest;

int main(void)
{

	int key_mode = 0;	//标示现在按键所处模式，由key2进行切换。0：微调pitch模式，1：微调roll模式
  
   // int i;
	  SystemClock_HSI(9);    //系统时钟设置,9倍频，36M
    NVIC_INIT();	                //中断初始化
    UART1_init(SysClock,115200);
    STMFLASH_Unlock();            //内部flash解锁
    LedInit();						 //LED 初始化	
    Adc_Init();            //摇杆AD初始化
    NRF24L01_Init();       //无线模块初始化
		TX_Mode();						 //发送模式
    TIM4_Init(SysClock,1000);           //定时器初始化,定时周期为1ms，1ms发送一次摇杆AD值
    TIM3_Init(SysClock,5000);           //打印调试信息
    KeyInit();             //按键初始化
    TxBuf[30]=Syn_byte;    //对应遥控器，该位写0xA5,保证飞机能在开机的时候能收到一个完整的32字节的数据包
    GetAD(R_Mode);          
/*******************************************************/
//以下为遥控解锁段         
/*******************************************************/
     while((Throttle>5)||(Roll>5))//解锁条件：油门拉到最低，方向打到最左解锁
     GetAD(R_Mode);//得到各路AD值并显示
    
     TxBuf[28]=ignore_parWrite;     //对应遥控器，该字节写0，使飞机跳过写参数那一步
     TxBuf[27]=Unlock;              //解锁飞机
     TxBuf[31]=EnableAircraft;      //解锁成功，使能飞控
/*******************************************************/
//解锁完成
/*******************************************************/
    
   Led1=0;
   Led2=0;
   Led3=0;
   Led4=0;
   Led5=0;
     
 	while(1)
	{  
      
    GetAD(R_Mode);    //得到各路AD，中断发送
    
    if(KeyScan() == kSet_Mode_Choose){
		if(0 == key_mode){
			key_mode = 1;
			Led2 = 1;	//Led2亮起标示进入了微调roll的模式
		}else{
			key_mode = 0;
			Led2 = 0;	//Led2灭掉标示进入了微调pitch的模式
		}
	}
       
    if(KeyScan() == kValueinc){
		if(0 == key_mode)	//微调pitch
		{
			Pitch_Offest++;
		}else{	//微调roll
			Roll_Offest++;
		}

	 	Led3 = 1;
	}
      
    if(KeyScan() == kValuedec){
		if(0 == key_mode)	//微调pitch
		{
			Pitch_Offest--;
		}else{	//微调roll
			Roll_Offest--;
		}

	 	Led4 = 1;
	}

	printf("pitch offset %d\r\n",(int) Pitch_Offest);
	printf("roll offset %d\r\n",(int) Roll_Offest);
    
	
	//遥控上的Key1暂时不用，余下的三个按键自定义，可以设置为偏航调节，一键动作等等
      
//       
//    
// //测试环形缓冲数组用，可以无视或者直接注释掉。不注释也不影响操作
//     printf("\r\n收到数据[%d] = %d\r\n",i,rx_buffer[i++]);
//     printf("读指针 = %d\r\n",UartRxbuf.Rd_Indx & UartRxbuf.Mask);
//     printf("写指针 = %d\r\n",UartRxbuf.Wd_Indx & UartRxbuf.Mask);
//     printf("可用%d字节\r\n",UartBuf_Cnt(&UartRxbuf));
//     if(i>UartRxbuf.Mask){
//     i=0;
//     //UartBuf_RD(&UartRxbuf);
//     printf("*********************\r\n");
//     }
// /////////////////////////////////////////////////////////////////   
//       
//       
     
		}
}





