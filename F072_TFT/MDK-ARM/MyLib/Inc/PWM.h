#ifndef __PWM_H
#define __PWM_H

#include "main.h"

#define PWM_CURSOR_X_MAX 6
#define PWM_CURSOR_Y_MAX 2

#define PWM_FREQUENCY_CURSOR_Y 0
#define PWM_DUTYCYCLE_CURSOR_Y 1

typedef struct
{
	Value_TypeDef Frequency;			//Ä¬ÈÏ1KHz  µ¥Î»
	Value_TypeDef DutyCycle;		
}PWM_TypeDef;

typedef struct
{
	PWM_TypeDef pwm;
}PWM_namespace;

extern PWM_TypeDef pwm;
extern Cell_Typedef pwm_cells[PWM_CURSOR_Y_MAX][PWM_CURSOR_X_MAX];

void PWM_cellInit(PWM_TypeDef *p_pwm);
void PWM_interfaceInit(PWM_TypeDef *p_pwm);
void PWM_dataInit(PWM_TypeDef *p_pwm);

void PWM_interfaceUpdate(PWM_TypeDef *p_pwm);

void PWM_updateOutput(PWM_TypeDef *p_pwm);
#endif
