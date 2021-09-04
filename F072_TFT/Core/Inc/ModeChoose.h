#ifndef __MODECHOOSE_H
#define __MODECHOOSE_H

#include "main.h"

typedef enum
{
	KEY_CHECKING = 0,
	KEY_CONFIRMING,
	KEY_RELEASING
}KEY_STATE;

typedef struct
{
	GPIO_TypeDef* GPIO_Port;
	uint16_t GPIO_Pin;
	KEY_STATE state;
	volatile uint8_t  button_flag;
	volatile uint16_t hold_flag;
}Key_TypeDef;

extern Key_TypeDef Key_1,Key_2,Key_R,Key_O,Key_L;

void Key_Init(void);
void Key_Check(Key_TypeDef *key_state);
#endif
