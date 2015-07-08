/**
* EEPROM Table by samit
**/

#include "ConfigTable.h"
#include "stmflash.h"
#include "Control.h"
#include "SysConfig.h"
#include "NRF24L01.h"
//
#define TABLE_ADDRESS (STM32_FLASH_BASE+STM32_FLASH_OFFEST+0)
//用来存放EEPROM列表上的存放的参数变量的信息
config_table_t table;				     //tobe improved: config mean in const / eeprom.
//请求保存参数到EEPROM的信号量
uint8_t gParamsSaveEEPROMRequset=0;

#define EEPROM_DEFAULT_VERSION 1





extern uint16_t Throttle_Calibra;
extern uint16_t Pitch_Calibra;
extern uint16_t Roll_Calibra;
extern uint16_t Yaw_Calibra ;//摇杆校准值



static uint8_t  isEEPROMValid(void)
{
	STMFLASH_Read(TABLE_ADDRESS,(uint16_t *)(&table),2);
	if((uint16_t)table.version==EEPROM_DEFAULT_VERSION)
			return 1;
	else
			return 0;
}

//table defalat . if 
void TableResetDefault(void)
{
		STMFLASH_Write(TABLE_ADDRESS,(uint16_t *)(&(table.version)),2);
}
//load params from EEPROM
void TableReadEEPROM(void)
{
		uint8_t paramNums=sizeof(table)/sizeof(uint16_t);
		STMFLASH_Read(TABLE_ADDRESS,(uint16_t *)(&table),paramNums * 2);
}

//write params to EEPROM
void TableWriteEEPROM(void)
{
		uint8_t paramNums=sizeof(table)/sizeof(uint16_t);

		STMFLASH_Write(TABLE_ADDRESS,(uint16_t *)(&table),paramNums * 2);
}

void TableToParam(void)
{
	    
	    Throttle_Calibra = table.thrCalibraVaule;
			Pitch_Calibra = table.pitCalibraVaule;
	    Roll_Calibra = table.rollCalibraVaule;
	    Yaw_Calibra = table.yawCalibraVaule;
	    ClibraFlag  = table.Clibra_flag;
			TX_ADDRESS[4] = (uint8_t)table.tx_addr;
}

void ParamToTable(void)
{
	table.version          = EEPROM_DEFAULT_VERSION;
	table.thrCalibraVaule  = Throttle_Calibra;
	table.pitCalibraVaule  = Pitch_Calibra;
	table.rollCalibraVaule = Roll_Calibra;
	table.yawCalibraVaule  = Yaw_Calibra;
	table.Clibra_flag      = ClibraFlag;
	table.tx_addr					 = (uint16_t)TX_ADDRESS[4];

}

void LoadParamsFromEEPROM(void)
{
	if(isEEPROMValid())
	{
			TableReadEEPROM();
			TableToParam();
		
	}
	else
	{
		
			ParamSetDefault();//版本检测不对，各项参数设为默认值
			ParamToTable();
			table.version=EEPROM_DEFAULT_VERSION;
			TableWriteEEPROM();
	}
}

void SaveParamsToEEPROM(void)
{
		ParamToTable();
		TableWriteEEPROM();
}

//all default value 
void ParamSetDefault(void) 
{
	    
			Throttle_Calibra 			= 0;
			Pitch_Calibra 				= 0;
	    Roll_Calibra 					= 0;
	    Yaw_Calibra 					= 0;
	    ClibraFlag            = FAIL;
			TX_ADDRESS[4]					= 0x00;
}
