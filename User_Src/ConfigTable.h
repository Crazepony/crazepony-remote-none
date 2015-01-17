#ifndef __CONFIG_TABLE_H
#define __CONFIG_TABLE_H
//
#include "stm32f10x.h"



#define OK   0xa5
#define FAIL 0x5a


typedef struct config_table_tt
{
uint16_t version;
uint16_t thrCalibraVaule;
uint16_t pitCalibraVaule;
uint16_t rollCalibraVaule;
uint16_t yawCalibraVaule;
uint16_t Clibra_flag;
}config_table_t;

extern config_table_t table;	
extern uint8_t gParamsSaveEEPROMRequset;

void LoadParamsFromEEPROM(void);
void ParamSetDefault(void) ;
void ParamToTable(void);
void TableToParam(void);
void TableWriteEEPROM(void);
void TableReadEEPROM(void);
void SaveParamsToEEPROM(void);

#endif
