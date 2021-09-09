#include "main.h"

Key_TypeDef Key_1,Key_2,Key_R,Key_O,Key_L;
uint8_t button_state;
uint8_t output_state;

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

void modeCheck(void)
{
	if(button_state == NO_KEY_PRESSED)
		return;
	else if(button_state == KEY_1_PRESSED)		//切换模式
	{
		//Cursor Init
		cursor_x = 0;
		cursor_y = 0;
		if(output_state == AWG_OUTPUT_MODE)
		{
			output_state = PWM_OUTPUT_MODE;
			LCD_Clear(BLACK);
			PWM_interfaceInit(&pwm);
		}
		else
		{
			output_state = AWG_OUTPUT_MODE;
			LCD_Clear(BLACK);
			awg.Mode = AWG_MODE_SINE;
			AWG_interfaceInit();
		}
	}
	else	//Key_2 O R L按下
	{
		if(output_state == AWG_OUTPUT_MODE)
			AWG_interfaceUpdate();
		else
			PWM_interfaceUpdate(&pwm);
	}
}
