#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "main.h"

#define CELL_MODE_NUM 0
#define CELL_MODE_TXT 1

#define NOT_SELECTED  0
#define IS_SELECTED   1

#define FONT_W 8
#define FONT_H 16

#define CURSOR_X_MAX 6
#define CURSOR_Y_MAX 6

#define FREQUENCY_CURSOR_Y	2
#define PERIOD_CURSOR_Y			3
#define AMPLITUDE_CURSOR_Y	4
#define OFFSET_CURSOR_Y			5

#define INT_CURSOR_K				0		//千位
#define INT_CURSOR_H				1		//百位
#define INT_CURSOR_T				2		//十位
#define INT_CURSOR_O				3		//个位

typedef struct
{
	uint16_t LCD_X;
	uint16_t LCD_Y;
	short*	 p_num;
	short		 num_min;
	short    num_max;
	char*		 text;
	uint8_t  len;
	uint8_t  cell_mode;		//为0表示输出数字，为1表示输出字符串
	uint8_t  is_selected; //为0表示该cell未被选中，为1表示被选中
}Cell_Typedef;

void Cell_TFT_Show(Cell_Typedef *cell);
uint8_t Cell_getLength(Cell_Typedef* cell);
void Cell_init(void);

void AWG_interfaceInit(void);
void interfaceUpdate(void);
//void valueIncrease(Value_TypeDef *value);
//void valueDecrease(Value_TypeDef *value);
void resetNum(short *num, short min_num, short max_num);
void Cell_numIncrease(Cell_Typedef *cell);
void Cell_numDecrease(Cell_Typedef *cell);

#endif
