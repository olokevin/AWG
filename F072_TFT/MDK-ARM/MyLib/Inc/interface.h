#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "main.h"

#define CELL_MODE_NUM 0
#define CELL_MODE_TXT 1

#define NOT_SELECTED  0
#define IS_SELECTED   1

#define FONT_W 8
#define FONT_H 16

#define INT_CURSOR_K				0		//ǧλ
#define INT_CURSOR_H				1		//��λ
#define INT_CURSOR_T				2		//ʮλ
#define INT_CURSOR_O				3		//��λ

typedef struct
{
	uint16_t LCD_X;
	uint16_t LCD_Y;
	short*	 p_num;
	short		 num_min;
	short    num_max;
	char*		 text;
	uint8_t  len;
	uint8_t  cell_mode;		//Ϊ0��ʾ������֣�Ϊ1��ʾ����ַ���
	uint8_t  is_selected; //Ϊ0��ʾ��cellδ��ѡ�У�Ϊ1��ʾ��ѡ��
}Cell_Typedef;

extern short cursor_x;
extern short cursor_y;

void Cell_TFT_Show(Cell_Typedef *cell);
uint8_t Cell_getLength(Cell_Typedef* cell);

//void valueIncrease(Value_TypeDef *value);
//void valueDecrease(Value_TypeDef *value);
void resetNum(short *num, short min_num, short max_num);
void Cell_numIncrease(Cell_Typedef *cell);
void Cell_numDecrease(Cell_Typedef *cell);

#endif
