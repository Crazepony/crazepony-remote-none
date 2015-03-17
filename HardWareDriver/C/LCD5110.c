/*****************************************************************
 * 文件名   ：LCD5110.c
 * 描述     ：LDC5110的驱动程序，仅包含ASCII字符及字符窜的显示功能    
 * 实验平台 ：STM32f103T8U6
 * 库版本   ：ST3.5.0
 * 硬件连接 ：IO口模拟  GPIOB
 * 	     _____________________________
 * 	     STM32_I/O    ----    LCD5110
 *
 *	     GPIOD_Pin_3 ---->    SCLK
 *	     
 *	     GPIOD_Pin_4 ---->    SDIN
 *	     
 *	     GPIOD_Pin_5  ---->    DC
 *	     
 *	     GPIOD_Pin_6  ---->    CE
 *	     
 *	     GPIOD_Pin_7  ---->    RST
 *           _____________________________
 *
 * 作者     ：MJ
 * 时间     ：2014-2-18 0:47:33
 * 情况     ：可以使用
 * 其它     ：今天查到考研成绩，没考上==
 ******************************************************************/




#include "LCD5110.h"
#include "delay.h"
#include "english_6x8_pixel.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
/************************************************************
函数名称：GPIO_Configuration
功    能：配置相关IO口
参    数：无
返回值  ：无
说    明：main中调用
************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LCD_PORT, ENABLE);
  RCC->APB2ENR|=1<<0;    //使能复用时钟	   
	
	
	GPIO_InitStructure.GPIO_Pin = SCLK_PIN|SDIN_PIN|DC_PIN|CE_PIN|RST_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出频率最大50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //带上拉输出
	GPIO_Init(LCD_GPIO, &GPIO_InitStructure); //初始化
  AFIO->MAPR|=2<<24;      //关闭JATG,千万不能讲SWD也关闭，否则芯片作废，亲测!!!!!!!!!!!!!!!!!!!!!!!!!!!!

}



/******************************************************************
函数名称：LCD_Write_Byt
功    能：向LCD5110的寄存器内写入一字节的数据
参    数：dat --- 数据内容 ； 
	  com --- 数据形式（0代表写入指令，1代表写入到数据寄存区）
返回值  ：无
说    明：仅在本文件中调用
*******************************************************************/
void LCD_Write_Byte(u8 dat,u8 com)
{
	u8 i;
	CE0;
	if(com==0)
		{DC0;}
	else
		{DC1;}

	for(i=0;i<8;i++)
	{
		if(dat&0x80)
			{SDIN1;}
		else
			{SDIN0;}
		SCLK0;
		dat=dat<<1;
		Delay(20);
		SCLK1;
	}
	CE1;	
}

/************************************************************
函数名称：LCD_Clear
功    能：LCD整屏清除
参    数：无
返回值  ：无
说    明：main中调用或本文件中的LCD_Init初始化函数中调用
************************************************************/
void LCD_Clear(void)
{
    u16 i;

    LCD_Write_Byte(0x0c, 0);			
    LCD_Write_Byte(0x80, 0);			

    for (i=0; i<504; i++)
    LCD_Write_Byte(0, 1);			
}





/************************************************************
函数名称：LCD_Init
功    能：
         1.初始化液晶设置
参    数：无
返回值  ：无
说    明：main中调用
************************************************************/
void LCD_Init(void)
{
	Delay(100);
  RST0;
	Delay(100);
	RST1;
	Delay(100);
	CE0;
	Delay(100);
	CE1;
	Delay(100);
	LCD_Write_Byte(0x21,0);
	LCD_Write_Byte(0xc8,0);
	LCD_Write_Byte(0x06,0);
	LCD_Write_Byte(0x13,0);
	LCD_Write_Byte(0x20,0);
	LCD_Clear();
	CE0;
}

/************************************************************
函数名称：LCD_On
功    能：1.初始化液晶所在IO
          2.开启液晶
	      3.初始化液晶设置
参    数：无
返回值  ：无
说    明：main中调用
************************************************************/
void LCD_On()
{
    GPIO_Configuration();
    LCD_Init();
    LCD_Clear();      
}

/************************************************************
函数名称：LCD_Off
功    能：1.关闭LCD
参    数：无
返回值  ：无
说    明：main中调用
************************************************************/
void LCD_Off()
{
     GPIO_ResetBits(LCD_GPIO,SCLK_PIN|SDIN_PIN|DC_PIN|CE_PIN|RST_PIN);
}

/************************************************************
函数名称：LCD_Set_XY
功    能：设置LCD5110的光标位置
参    数：X --- 行坐标（第X行）； Y --- 列坐标 （第Y咧）
	  X、Y 均从0开始
返回值  ：无
说    明：main中或者本文件中调用
************************************************************/
void LCD_Set_XY(u8 X,u8 Y)
{
    LCD_Write_Byte(0x40 | X, 0);		// column
    LCD_Write_Byte(0x80 | Y*6, 0);  // row
}



/************************************************************************
函数名称：LCD_Write_Charn
功    能：LCD上当前光标处显示一个字符
参    数：c --- 对应要显示的字符的ASCII码
返回值  ：无
说    明：显示函数调用了字符的库，所以必须要包含english_6x8_pixel.h文件
*************************************************************************/
void LCD_Write_Char(u8 c)
{
    u8 line;
    c-=32;
    for (line=0; line<6; line++)
    LCD_Write_Byte(font6x8[c][line], 1);
}




/************************************************************
函数名称：LCD_write_english_string
功    能：指定坐标(X,Y)处开始连续输出一个字符串
参    数：(X,Y) --- 指定光标位置
返回值  ：无
说    明：本函数不具备自动换行功能
************************************************************/
void LCD_write_english_string(u8 X,u8 Y,char *s)
{
    LCD_Set_XY(X,Y);
    while (*s) 
      {
         LCD_Write_Char(*s);
         s++;
      }
}





