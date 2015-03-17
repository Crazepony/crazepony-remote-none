#ifndef __LCD5110_H__
#define __LCD5110_H__

#include "stm32f10x.h"

/*GPIO_PIN口定义*/
#define SCLK_PIN GPIO_Pin_3
#define SDIN_PIN GPIO_Pin_4
#define DC_PIN   GPIO_Pin_5
#define CE_PIN   GPIO_Pin_6
#define RST_PIN  GPIO_Pin_7


/*GPIO组定义*/
#define RCC_APB2Periph_LCD_PORT   RCC_APB2Periph_GPIOB
#define LCD_GPIO GPIOB


/*IO电平设定宏定义*/
#define	SCLK1 GPIO_SetBits(LCD_GPIO, SCLK_PIN); 
#define	SCLK0 GPIO_ResetBits(LCD_GPIO, SCLK_PIN); 
#define	SDIN1 GPIO_SetBits(LCD_GPIO, SDIN_PIN); 
#define	SDIN0 GPIO_ResetBits(LCD_GPIO, SDIN_PIN);
#define	DC1 GPIO_SetBits(LCD_GPIO, DC_PIN); 
#define	DC0 GPIO_ResetBits(LCD_GPIO, DC_PIN);
#define	CE1 GPIO_SetBits(LCD_GPIO, CE_PIN); 
#define	CE0 GPIO_ResetBits(LCD_GPIO, CE_PIN);
#define	RST1 GPIO_SetBits(LCD_GPIO, RST_PIN); 
#define	RST0 GPIO_ResetBits(LCD_GPIO, RST_PIN);

/*可供外部调用的函数声明*/

void LCD_Write_Byte(u8 dat,u8 com);	/*dat为向5110中写入一个字节的数据，
				       	  若com等于1则表示写入的是数据；
				       	  若com等于0则表示写入的是命令*/

void LCD_Clear(void);			/*全屏清除函数*/

void LCD_Init(void);			/*液晶初始化函数*/

void LCD_Set_XY(u8 X,u8 Y);		/*设定下一显示位置的行列坐标，行和
					  列均从0开始*/

void LCD_Write_Char(u8 c);		/*向液晶上当前光标位置输出一个字符*/


/*在液晶上的(X,Y)坐标处显示一个字符串*/
void LCD_write_english_string(u8 X,u8 Y,char *s);
					
void LCD_On(void);/*液晶开启*/

void LCD_Off(void);/*关闭液晶*/



#endif


