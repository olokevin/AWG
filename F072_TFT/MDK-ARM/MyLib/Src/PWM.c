#include "main.h"

PWM_TypeDef pwm;
uint16_t PWM_AVAIL_CURSOR_X_MAX[PWM_CURSOR_Y_MAX] = {5,2};
uint16_t PWM_LCD_Y[PWM_CURSOR_Y_MAX] = {90,120};
uint16_t PWM_LCD_X[PWM_CURSOR_Y_MAX][PWM_CURSOR_X_MAX] = {
	{100,108,116,132,140,156},		//Frequency
	{116,124,  0,  0,  0,  0}			//DutyCycle
};

Cell_Typedef pwm_cells[PWM_CURSOR_Y_MAX][PWM_CURSOR_X_MAX];

void PWM_cellInit(PWM_TypeDef *p_pwm)
{	
	//Contents init
	pwm_cells[0][0].cell_mode = CELL_MODE_NUM;
	pwm_cells[0][0].p_num = &(p_pwm->Frequency.show_value_int[2]);	
	pwm_cells[0][0].num_min = 0;
	pwm_cells[0][0].num_max = 9;
	pwm_cells[0][0].is_selected = IS_SELECTED;
	pwm_cells[0][1].cell_mode = CELL_MODE_NUM;
	pwm_cells[0][1].p_num = &(p_pwm->Frequency.show_value_int[1]);	
	pwm_cells[0][1].num_min = 0;
	pwm_cells[0][1].num_max = 9;
	pwm_cells[0][2].cell_mode = CELL_MODE_NUM;
	pwm_cells[0][2].p_num = &(p_pwm->Frequency.show_value_int[0]);	
	pwm_cells[0][2].num_min = 0;
	pwm_cells[0][2].num_max = 9;
	pwm_cells[0][3].cell_mode = CELL_MODE_NUM;
	pwm_cells[0][3].p_num = &(p_pwm->Frequency.show_value_dec);
	pwm_cells[0][3].num_min = 0;
	pwm_cells[0][3].num_max = 9;
	pwm_cells[0][4].cell_mode = CELL_MODE_TXT;
	pwm_cells[0][4].text = p_pwm->Frequency.unit_string[p_pwm->Frequency.unit_no];

	pwm_cells[1][0].cell_mode = CELL_MODE_NUM;
	pwm_cells[1][0].p_num = &(p_pwm->DutyCycle.show_value_int[1]);
	pwm_cells[1][0].num_min = 0;
	pwm_cells[1][0].num_max = 9;
	pwm_cells[1][1].cell_mode = CELL_MODE_NUM;
	pwm_cells[1][1].p_num = &(p_pwm->DutyCycle.show_value_int[0]);
	pwm_cells[1][1].num_min = 0;
	pwm_cells[1][1].num_max = 9;
	
}

void PWM_interfaceInit(PWM_TypeDef *p_pwm)
{
	TFT_ShowString(15,45,"PWM Wave Generator",12,24,0,BLACK,WHITE);
	
	TFT_ShowString(15,PWM_LCD_Y[PWM_FREQUENCY_CURSOR_Y],"Frequency:",8,16,0,BLACK,WHITE);	
	TFT_ShowChar(124,PWM_LCD_Y[PWM_FREQUENCY_CURSOR_Y],'.',8,16,0,BLACK,WHITE);
	
	TFT_ShowString(15,PWM_LCD_Y[PWM_DUTYCYCLE_CURSOR_Y],"DutyCycle:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(132,PWM_LCD_Y[PWM_DUTYCYCLE_CURSOR_Y],'%',8,16,0,BLACK,WHITE);
	
	//Coordinate and length init&show
	int i,j;
	for(i=0;i<PWM_CURSOR_Y_MAX;i++)
	{
		for(j=0;j<PWM_AVAIL_CURSOR_X_MAX[i];j++)
		{
			pwm_cells[i][j].LCD_X = PWM_LCD_X[i][j];
			pwm_cells[i][j].LCD_Y = PWM_LCD_Y[i];
//			pwm_cells[i][j].len  = Cell_getLength(&pwm_cells[i][j]);
			Cell_TFT_Show(&pwm_cells[i][j]);
		}
	}
}

void PWM_dataInit(PWM_TypeDef *p_pwm)
{
	p_pwm->Frequency.actual_value = 100;
	p_pwm->Frequency.show_value_int[2] = 1;
	p_pwm->Frequency.show_value_int[1] = 0;
	p_pwm->Frequency.show_value_int[0] = 0;
	p_pwm->Frequency.show_value_dec = 0;
	p_pwm->Frequency.unit_no = 0;
	p_pwm->Frequency.unit_string[0] = "Hz ";
	p_pwm->Frequency.unit_string[1] = "kHz";
	
	p_pwm->DutyCycle.actual_value = 50;
	p_pwm->DutyCycle.show_value_int[1] = 5;
	p_pwm->DutyCycle.show_value_int[0] = 0;
}

void PWM_interfaceUpdate(PWM_TypeDef *p_pwm)
{
	switch(button_state)
	{
		case KEY_2_PRESSED:
			pwm_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
			Cell_TFT_Show(&pwm_cells[cursor_y][cursor_x]);
			
			cursor_y++;
			resetNum(&cursor_y, 0, PWM_CURSOR_Y_MAX-1);
				
			cursor_x = 0;
			
			pwm_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
			Cell_TFT_Show(&pwm_cells[cursor_y][cursor_x]);
			break;
		
		case KEY_O_PRESSED:
			pwm_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
			Cell_TFT_Show(&pwm_cells[cursor_y][cursor_x]);
			
			cursor_x++;
			resetNum(&cursor_x, 0, PWM_AVAIL_CURSOR_X_MAX[cursor_y]-1);
			
			pwm_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
			Cell_TFT_Show(&pwm_cells[cursor_y][cursor_x]);
			break;
		
		case KEY_R_PRESSED:
			switch(cursor_y)
			{
				case PWM_FREQUENCY_CURSOR_Y:
					if(pwm_cells[cursor_y][cursor_x].cell_mode == CELL_MODE_NUM)
					{
						Cell_numDecrease(&(pwm_cells[cursor_y][cursor_x]));
					}
					else
					{
						if(p_pwm->Frequency.unit_no == 0)
							p_pwm->Frequency.unit_no = 1;
						else
							p_pwm->Frequency.unit_no = 0;
						pwm_cells[cursor_y][cursor_x].text = p_pwm->Frequency.unit_string[p_pwm->Frequency.unit_no];
					}
					Cell_TFT_Show(&(pwm_cells[cursor_y][cursor_x]));
					
					p_pwm->Frequency.actual_value = p_pwm->Frequency.show_value_int[0]+\
																		10*p_pwm->Frequency.show_value_int[1]+\
																	 100*p_pwm->Frequency.show_value_int[2]+\
																	0.1f*p_pwm->Frequency.show_value_dec;
					if(p_pwm->Frequency.unit_no == 1)
						p_pwm->Frequency.actual_value *= 1000;
					break;
				
				case PWM_DUTYCYCLE_CURSOR_Y:
					Cell_numDecrease(&(pwm_cells[cursor_y][cursor_x]));
					Cell_TFT_Show(&(pwm_cells[cursor_y][cursor_x]));
					p_pwm->DutyCycle.actual_value = p_pwm->DutyCycle.show_value_int[0]+\
																			10*p_pwm->DutyCycle.show_value_int[1];
					break;
				default:
					break;
			}
			break;
			
		case KEY_L_PRESSED:
			switch(cursor_y)
			{
				case PWM_FREQUENCY_CURSOR_Y:
					if(pwm_cells[cursor_y][cursor_x].cell_mode == CELL_MODE_NUM)
					{
						Cell_numIncrease(&(pwm_cells[cursor_y][cursor_x]));
					}
					else
					{
						if(p_pwm->Frequency.unit_no == 0)
							p_pwm->Frequency.unit_no = 1;
						else
							p_pwm->Frequency.unit_no = 0;
						pwm_cells[cursor_y][cursor_x].text = p_pwm->Frequency.unit_string[p_pwm->Frequency.unit_no];
					}
					Cell_TFT_Show(&(pwm_cells[cursor_y][cursor_x]));
					p_pwm->Frequency.actual_value = p_pwm->Frequency.show_value_int[0]+\
																		10*p_pwm->Frequency.show_value_int[1]+\
																	 100*p_pwm->Frequency.show_value_int[2]+\
																	0.1f*p_pwm->Frequency.show_value_dec;
					if(p_pwm->Frequency.unit_no == 1)
						p_pwm->Frequency.actual_value *= 1000;
					break;
					
				case PWM_DUTYCYCLE_CURSOR_Y:
					Cell_numIncrease(&(pwm_cells[cursor_y][cursor_x]));
					Cell_TFT_Show(&(pwm_cells[cursor_y][cursor_x]));
					p_pwm->DutyCycle.actual_value = p_pwm->DutyCycle.show_value_int[0]+\
																			10*p_pwm->DutyCycle.show_value_int[1];
					break;
					
				default:
					break;
			}
			break;
		default:
			break;
	}
	//Êä³ö
	PWM_updateOutput(&pwm);
}

void PWM_updateOutput(PWM_TypeDef *p_pwm)
{
	uint16_t tim2_psc;
	//Stop Output
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);
	
	tim2_psc = 4800000 / p_pwm->Frequency.actual_value - 1;
	TIM3->CNT = 0;
	__HAL_TIM_SET_PRESCALER(&htim2, tim2_psc);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3 ,p_pwm->DutyCycle.actual_value);

	//Start Output
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
}
