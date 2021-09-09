#ifndef __AWG_H
#define __AWG_H

#include "main.h"

//DAC defines
#define SAMPLE_POINTS 240
#define DAC_DHR12RD_ADDRESS (DAC_BASE+0x20)		//12λ�Ҷ���Ĵ�����ַ

#define CURSOR_X_MAX 6
#define CURSOR_Y_MAX 4

#define MODE_CURSOR_Y				0
#define FREQUENCY_CURSOR_Y	1
#define AMPLITUDE_CURSOR_Y	2
#define OFFSET_CURSOR_Y			3

#define DAC_MIN 256
#define DAC_MAX 3840

typedef enum
{
	AWG_MODE_SINE = 0,
	AWG_MODE_TRI,
	AWG_MODE_ZIG	
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
	float	actual_value;			//ʵ��ֵ,��λΪHz,ms,V
	short	show_value_int[3];		//��ʾ����ֵ ��λ�±�Ϊ2 ��λ�±�Ϊ0
	short	show_value_dec;		//��ʾС��ֵ
	short		unit_no;	//��λ���
	char		*unit_string[2];
}Value_TypeDef;

typedef struct
{
	AWG_MODE_ENUM Mode;
	float max_output;
	Value_TypeDef Frequency;			//Ĭ��1KHz  ��λ
	Value_TypeDef Period;				
	Value_TypeDef Amplitude;	
	Value_TypeDef Offset;	
}AWG_TypeDef;

extern uint16_t sine_data[SAMPLE_POINTS];
extern uint16_t tri_data[SAMPLE_POINTS];
extern uint16_t dac_output[SAMPLE_POINTS];

extern AWG_TypeDef awg;

extern uint16_t AVAIL_CURSOR_X_MAX[CURSOR_Y_MAX];
extern uint16_t AWG_LCD_Y[CURSOR_Y_MAX];
extern uint16_t AWG_LCD_X[CURSOR_Y_MAX][CURSOR_X_MAX];

extern Cell_Typedef awg_cells[CURSOR_Y_MAX][CURSOR_X_MAX];		//��Ϊy ��Ϊx

void AWG_dataInit(void);
void AWG_cellInit(void);
void AWG_interfaceInit(void);

void AWG_interfaceUpdate(void);

void AWG_dacUpdate(AWG_TypeDef *p_awg, uint16_t* origin_array, uint16_t* output_array);
void AWG_updateOutput(AWG_TypeDef *p_awg);


#endif
