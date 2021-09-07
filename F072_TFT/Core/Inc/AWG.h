#ifndef __AWG_H
#define __AWG_H

#include "main.h"

//DAC defines
#define SAMPLE_POINTS 240
#define DAC_DHR12RD_ADDRESS (DAC_BASE+0x20)		//12位右对齐寄存器地址
#define ORIGIN_AMP 3.5

typedef enum
{
	AWG_MODE_SINE = 0,
	AWG_MODE_TRI,
	AWG_MODE_PWM	
}AWG_MODE_ENUM;

typedef enum
{
	AWG_CURSOR_NONE = 0,
	AWG_CURSOR_MODE,
	AWG_CURSOR_FREQ,
	AWG_CURSOR_PERIOD,
	AWG_CURSOR_AMP,
	AWG_CURSOR_DUTY,
	AWG_CURSOR_OFFSET
}CURSOR_ENUM;

typedef struct
{
	float	actual_value;			//实际值,单位为Hz,ms,V
	short	show_value_int[3];		//显示整数值 百位下标为2 个位下标为0
	short	show_value_dec;		//显示小数值
	short		unit_no;	//单位编号
	char		*unit_string[2];
}Value_TypeDef;

typedef struct
{
	CURSOR_ENUM Cursor;
	AWG_MODE_ENUM Mode;
	Value_TypeDef Frequency;			//默认1KHz  单位
	Value_TypeDef Period;				
	Value_TypeDef Amplitude;
	Value_TypeDef DutyCycle;	
	Value_TypeDef Offset;	
}AWG_TypeDef;

extern uint16_t sine_data[SAMPLE_POINTS];
extern uint16_t tri_data[SAMPLE_POINTS];
extern uint16_t dac_output[SAMPLE_POINTS];

extern AWG_TypeDef awg;

void AWG_Init(void);
void AWG_dacUpdate(AWG_TypeDef *p_awg, uint16_t* origin_array, uint16_t* output_array);
void AWG_PWMUpdate(AWG_TypeDef *p_awg);
void AWG_updateOutput(AWG_TypeDef *p_awg);

#endif
