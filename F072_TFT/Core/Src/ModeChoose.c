#include "main.h"

Key_TypeDef Key_1,Key_2,Key_R,Key_O,Key_L;
uint8_t button_state;

void Key_Init(void)
{
	Key_1.GPIO_Port = Key_1_GPIO_Port;
	Key_1.GPIO_Pin  = Key_1_Pin;
	Key_1.state 		= KEY_CHECKING;
	
	Key_2.GPIO_Port = Key_2_GPIO_Port;
	Key_2.GPIO_Pin  = Key_2_Pin;
	Key_2.state 		= KEY_CHECKING;
	
	Key_R.GPIO_Port = Key_R_GPIO_Port;
	Key_R.GPIO_Pin  = Key_R_Pin;
	Key_R.state 		= KEY_CHECKING;
	
	Key_O.GPIO_Port = Key_O_GPIO_Port;
	Key_O.GPIO_Pin  = Key_O_Pin;
	Key_O.state 		= KEY_CHECKING;
	
	Key_L.GPIO_Port = Key_L_GPIO_Port;
	Key_1.GPIO_Pin  = Key_L_Pin;
	Key_1.state 		= KEY_CHECKING;
}

/*
C++才可以用结构体的引用作为形参
如：void Key_Check(Key_TypeDef &key_state)
C语言不可以，只能指针传参
*/
void Key_Check(Key_TypeDef *key_state)
{
	switch(key_state->state)
	{
		case KEY_CHECKING:
		{
			if(HAL_GPIO_ReadPin(key_state->GPIO_Port, key_state->GPIO_Pin) == GPIO_PIN_SET)
		{
			key_state->state = KEY_CONFIRMING;
		}
		break;
		}
		
		case KEY_CONFIRMING:
		{
			if(HAL_GPIO_ReadPin(key_state->GPIO_Port, key_state->GPIO_Pin) == GPIO_PIN_SET)
			{
				key_state->button_flag = 1;
				key_state->state = KEY_RELEASING;
			}
			else if(HAL_GPIO_ReadPin(key_state->GPIO_Port, key_state->GPIO_Pin) == GPIO_PIN_RESET)
			{
				key_state->state = KEY_CHECKING;
			}
			break;
		}
		
		case KEY_RELEASING:
		{
			if(HAL_GPIO_ReadPin(key_state->GPIO_Port, key_state->GPIO_Pin) == GPIO_PIN_RESET)
			{
				key_state->state = KEY_CHECKING;
			}
			else
			{
				key_state->state = KEY_RELEASING;
				key_state->hold_flag++;
			}
			break;
		}		
		default: break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{		
	if(htim->Instance == TIM7)
	{
		Key_Check(&Key_1);
		Key_Check(&Key_2);
		Key_Check(&Key_R);
		Key_Check(&Key_O);
		Key_Check(&Key_L);
	}
}

/**
  * @brief           Pass the current button state to the upper layer.
  * @note            Support debounce, short press(less than 1s) and long press(hold, more than 1s).
  * @param[in, out]  button_state  Encoded button state with long press flag added.
  *                    Bit5: Long press(hold) flag.
  * @retval          None
  */
void read_button(uint8_t *button_state)
{
	uint8_t i;
	if (*button_state != NO_KEY_PRESSED && encode_button())
	{
		*button_state = 0xFF;
		return ;
	}

	// Read button states.
	*button_state = encode_button();
	if (*button_state == NO_KEY_PRESSED)
		return ;  // None of the buttons has been pressed.

	HAL_Delay(50);
	*button_state = encode_button();
	if (*button_state == NO_KEY_PRESSED)
		return ;  // None of the buttons has been pressed.

	//1s以上算长按
	for (i = 0; i < 20; i++)
	{
		HAL_Delay(50);
		if (encode_button() == NO_KEY_PRESSED)  // The pressed button has been released.
			return ;
	}

	*button_state = 0x20 | encode_button();  // One or two buttons have been held, return the button state.
}

/**
  * @brief  Encode button state into a 5-bit integer.
  * @retval Encoded button state.
  * 		In every bit, 1 represents the corresponding button has been pressed,
			0 represents the corresponding button has not been pressed.
  *           Bit0: Button "OK"
  *           Bit1: Button "D"
  *           Bit2: Button "U"
  *           Bit3: Button "R"
  *           Bit4: Button "L"
  */
inline uint8_t encode_button(void)
{
	return (uint8_t)HAL_GPIO_ReadPin(Key_1_GPIO_Port, Key_1_Pin)<<4\
	| (uint8_t)HAL_GPIO_ReadPin(Key_2_GPIO_Port, Key_2_Pin) << 3 \
	| (uint8_t)HAL_GPIO_ReadPin(Key_R_GPIO_Port, Key_R_Pin) << 2 \
	| (uint8_t)HAL_GPIO_ReadPin(Key_O_GPIO_Port, Key_O_Pin) << 1 \
	| (uint8_t)HAL_GPIO_ReadPin(Key_L_GPIO_Port, Key_L_Pin);
}
