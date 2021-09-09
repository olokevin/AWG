#ifndef __MODECHOOSE_H
#define __MODECHOOSE_H

#include "main.h"

/*
高位->低位
Key_1 	Key_2 Key_R(左扳) Key_O Key_L(右扳)
长按标志
*/
#define NO_KEY_PRESSED 0x00

#define KEY_1_PRESSED 0x10
#define KEY_2_PRESSED 0x08
#define KEY_R_PRESSED 0x04
#define KEY_O_PRESSED 0x02
#define KEY_L_PRESSED 0x01

#define KEY_1_HOLD 0x30
#define KEY_2_HOLD 0x28
#define KEY_R_HOLD 0x24
#define KEY_O_HOLD 0x22
#define KEY_L_HOLD 0x21

#define AWG_OUTPUT_MODE		0
#define PWM_OUTPUT_MODE		1

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
extern uint8_t button_state;

void Key_Init(void);
void Key_Check(Key_TypeDef *key_state);

void read_button(uint8_t *button_state);
uint8_t encode_button(void);		//不用inline
void modeCheck(void);
#endif
