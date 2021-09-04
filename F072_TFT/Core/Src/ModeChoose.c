#include "main.h"

Key_TypeDef Key_1,Key_2,Key_R,Key_O,Key_L;

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

