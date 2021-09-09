#include "main.h"

short cursor_x;
short cursor_y;

uint16_t AVAIL_CURSOR_X_MAX[CURSOR_Y_MAX] = {3,5,3,3,2};
uint16_t AWG_LCD_Y[CURSOR_Y_MAX] = {90,120,150,180,210};

uint16_t AWG_LCD_X[CURSOR_Y_MAX][CURSOR_X_MAX] = {
	{ 80,130,180,  0,  0,  0},		//Mode:
	{100,108,116,132,140,156},		//Frequency
	{116,132,140,  0,  0,  0},		//Amplitude
	{116,132,140,  0,  0,  0},		//Offset
	{116,124,  0,  0,  0,  0}			//DutyCycle
};

Cell_Typedef awg_cells[CURSOR_Y_MAX][CURSOR_X_MAX];

uint8_t Cell_getLength(Cell_Typedef* cell)
{
	uint8_t temp_len = 0;
	uint32_t temp_num = 0;
	char* temp_text;
	if(cell->cell_mode == CELL_MODE_NUM)
	{
		temp_num = *(cell->p_num);
		temp_len = 1;
		while(temp_num / 10 != 0)
			temp_len++;
	}
	else if(cell->cell_mode == CELL_MODE_TXT)
	{
		temp_text = cell->text;
		while(*temp_text != '\0')
		{
			temp_len++;
			temp_text ++;
		}
	}
	return temp_len;
}

void Cell_TFT_Show(Cell_Typedef *cell)
{
	uint16_t back_color;
	uint16_t point_color;
	uint32_t show_num;
	//select the color
	if(cell->is_selected == IS_SELECTED)
	{
		back_color = WHITE;
		point_color = BLACK;
	}
	else
	{
		back_color = BLACK;
		point_color = WHITE;
	}
	
	//show mode
	if(cell->cell_mode == CELL_MODE_NUM)
	{
		if(*(cell->p_num)<0)
			show_num = -*(cell->p_num);
		else
			show_num = *(cell->p_num);
		TFT_ShowNum(cell->LCD_X, cell->LCD_Y, show_num, 1, FONT_W, FONT_H, back_color, point_color);
	}
	else if(cell->cell_mode == CELL_MODE_TXT)
		TFT_ShowString(cell->LCD_X, cell->LCD_Y, cell->text, FONT_W, FONT_H, 1, back_color, point_color);
}

/**
  * @brief  数字重设
  * @param
  * @retval
  */
void resetNum(short *num, short min_num, short max_num)
{
	if(*num < min_num)
		*num = max_num;
	else if(*num > max_num)
		*num = min_num;
}

/**
  * @brief  数字递增
  * @param
  * @retval
  */
void Cell_numIncrease(Cell_Typedef *cell)
{
	if(cell->cell_mode == CELL_MODE_NUM)
	{
		(*(cell->p_num))++;
		resetNum(cell->p_num, cell->num_min, cell->num_max);
	}
//	else if(cell->cell_mode == CELL_MODE_TXT)
//	{
//		cursor_x++;
//		resetNum(&cursor_x, 0, AVAIL_CURSOR_X_MAX[cursor_y]);
//	}
}
/**
  * @brief  数字递减
  * @param
  * @retval
  */
void Cell_numDecrease(Cell_Typedef *cell)
{
	if(cell->cell_mode == CELL_MODE_NUM)
	{
		(*(cell->p_num))--;
		resetNum(cell->p_num, cell->num_min, cell->num_max);
	}
}

void Cell_init(void)
{
	//Contents init	
	awg_cells[MODE_CURSOR_Y][0].cell_mode = CELL_MODE_TXT;
	awg_cells[MODE_CURSOR_Y][0].text = "Sine";
	awg_cells[MODE_CURSOR_Y][0].is_selected = IS_SELECTED;
	awg_cells[MODE_CURSOR_Y][1].cell_mode = CELL_MODE_TXT;
	awg_cells[MODE_CURSOR_Y][1].text = "Tri";	
	awg_cells[MODE_CURSOR_Y][2].cell_mode = CELL_MODE_TXT;
	awg_cells[MODE_CURSOR_Y][2].text = "PWM";
	
	awg_cells[FREQUENCY_CURSOR_Y][0].cell_mode = CELL_MODE_NUM;
	awg_cells[FREQUENCY_CURSOR_Y][0].p_num = &(awg.Frequency.show_value_int[2]);	
	awg_cells[FREQUENCY_CURSOR_Y][0].num_min = 0;
	awg_cells[FREQUENCY_CURSOR_Y][0].num_max = 9;
	awg_cells[FREQUENCY_CURSOR_Y][1].cell_mode = CELL_MODE_NUM;
	awg_cells[FREQUENCY_CURSOR_Y][1].p_num = &(awg.Frequency.show_value_int[1]);	
	awg_cells[FREQUENCY_CURSOR_Y][1].num_min = 0;
	awg_cells[FREQUENCY_CURSOR_Y][1].num_max = 9;
	awg_cells[FREQUENCY_CURSOR_Y][2].cell_mode = CELL_MODE_NUM;
	awg_cells[FREQUENCY_CURSOR_Y][2].p_num = &(awg.Frequency.show_value_int[0]);	
	awg_cells[FREQUENCY_CURSOR_Y][2].num_min = 0;
	awg_cells[FREQUENCY_CURSOR_Y][2].num_max = 9;
	awg_cells[FREQUENCY_CURSOR_Y][3].cell_mode = CELL_MODE_NUM;
	awg_cells[FREQUENCY_CURSOR_Y][3].p_num = &(awg.Frequency.show_value_dec);
	awg_cells[FREQUENCY_CURSOR_Y][3].num_min = 0;
	awg_cells[FREQUENCY_CURSOR_Y][3].num_max = 9;
	awg_cells[FREQUENCY_CURSOR_Y][4].cell_mode = CELL_MODE_TXT;
	awg_cells[FREQUENCY_CURSOR_Y][4].text = awg.Frequency.unit_string[awg.Frequency.unit_no];
	
	awg_cells[AMPLITUDE_CURSOR_Y][0].cell_mode = CELL_MODE_NUM;
	awg_cells[AMPLITUDE_CURSOR_Y][0].p_num = &(awg.Amplitude.show_value_int[0]);
	awg_cells[AMPLITUDE_CURSOR_Y][0].num_min = -4;
	awg_cells[AMPLITUDE_CURSOR_Y][0].num_max = 4;
	awg_cells[AMPLITUDE_CURSOR_Y][1].cell_mode = CELL_MODE_NUM;
	awg_cells[AMPLITUDE_CURSOR_Y][1].p_num = &(awg.Amplitude.show_value_dec);
	awg_cells[AMPLITUDE_CURSOR_Y][1].num_min = 0;
	awg_cells[AMPLITUDE_CURSOR_Y][1].num_max = 9;
	awg_cells[AMPLITUDE_CURSOR_Y][2].cell_mode = CELL_MODE_TXT;
	awg_cells[AMPLITUDE_CURSOR_Y][2].text = awg.Amplitude.unit_string[awg.Amplitude.unit_no];
	
	awg_cells[OFFSET_CURSOR_Y][0].cell_mode = CELL_MODE_NUM;
	awg_cells[OFFSET_CURSOR_Y][0].p_num = &(awg.Offset.show_value_int[0]);
	awg_cells[OFFSET_CURSOR_Y][0].num_min = -4;
	awg_cells[OFFSET_CURSOR_Y][0].num_max = 4;
	awg_cells[OFFSET_CURSOR_Y][1].cell_mode = CELL_MODE_NUM;
	awg_cells[OFFSET_CURSOR_Y][1].p_num = &(awg.Offset.show_value_dec);
	awg_cells[OFFSET_CURSOR_Y][1].num_min = 0;
	awg_cells[OFFSET_CURSOR_Y][1].num_max = 9;
	awg_cells[OFFSET_CURSOR_Y][2].cell_mode = CELL_MODE_TXT;
	awg_cells[OFFSET_CURSOR_Y][2].text = awg.Offset.unit_string[awg.Offset.unit_no];
	
	awg_cells[DUTYCYCLE_CURSOR_Y][0].cell_mode = CELL_MODE_NUM;
	awg_cells[DUTYCYCLE_CURSOR_Y][0].p_num = &(awg.DutyCycle.show_value_int[1]);
	awg_cells[DUTYCYCLE_CURSOR_Y][0].num_min = 0;
	awg_cells[DUTYCYCLE_CURSOR_Y][0].num_max = 9;
	awg_cells[DUTYCYCLE_CURSOR_Y][1].cell_mode = CELL_MODE_NUM;
	awg_cells[DUTYCYCLE_CURSOR_Y][1].p_num = &(awg.DutyCycle.show_value_int[0]);
	awg_cells[DUTYCYCLE_CURSOR_Y][1].num_min = 0;
	awg_cells[DUTYCYCLE_CURSOR_Y][1].num_max = 9;
	//Coordinate and length init&show
	int i,j;
	for(i=0;i<CURSOR_Y_MAX;i++)
	{
		for(j=0;j<AVAIL_CURSOR_X_MAX[i];j++)
		{
			awg_cells[i][j].LCD_X = AWG_LCD_X[i][j];
			awg_cells[i][j].LCD_Y = AWG_LCD_Y[i];
//			awg_cells[i][j].len  = Cell_getLength(&awg_cells[i][j]);
			Cell_TFT_Show(&awg_cells[i][j]);
		}
	}
	
}

void AWG_interfaceInit(void)
{
	TFT_ShowString(15,45,"Function Generator",12,24,0,BLACK,WHITE);
	
	TFT_ShowString(15,90,"Mode:",8,16,0,BLACK,WHITE);	
	
	TFT_ShowString(15,120,"Frequency:",8,16,0,BLACK,WHITE);	
	TFT_ShowChar(124,120,'.',8,16,0,BLACK,WHITE);	
	
	TFT_ShowString(15,150,"Amplitude:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(124,150,'.',8,16,0,BLACK,WHITE);
	
	TFT_ShowString(15,180,"Offset:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(124,180,'.',8,16,0,BLACK,WHITE);
	
	TFT_ShowString(15,210,"DutyCycle:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(132,210,'%',8,16,0,BLACK,WHITE);
}

//void interfaceKey_1_Update(void)
//{
//	
//}

//void interfaceKey_2_Update(void)
//{
//	
//}

//void interfaceKey_R_Update(void)
//{
//	
//}

//void interfaceKey_O_Update(void)
//{
//	
//}

//void interfaceKey_L_Update(void)
//{
//	
//}

void interfaceUpdate(void)
{
	switch(button_state)
	{
		case NO_KEY_PRESSED:
			return ;
			break;
		
		case KEY_1_PRESSED:
			break;
		
		case KEY_2_PRESSED:
			if(cursor_y == MODE_CURSOR_Y)		//从模式选择切换到下一行
				awg.Mode = cursor_x;					//模式为x轴光标值
			else														//其他行时，需要把上一次的切换回黑白显示
				awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
			Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
			
			cursor_y++;
			resetNum(&cursor_y, 0, CURSOR_Y_MAX-1);
			
			if(cursor_y == MODE_CURSOR_Y)		//现在切换到模式选择
				cursor_x = awg.Mode;					//x光标停留在当前的输出模式
			else														//其他情况下，x光标需归0
				cursor_x = 0;
			
			awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
			Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
			break;
		
		case KEY_O_PRESSED:
			awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
			Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
			
			cursor_x++;
			resetNum(&cursor_x, 0, AVAIL_CURSOR_X_MAX[cursor_y]-1);
			
			awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
			Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
			break;
		
		case KEY_R_PRESSED:
			switch(cursor_y)
			{
				case MODE_CURSOR_Y:
					awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
					Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
					
					cursor_x--;
					resetNum(&cursor_x, 0, AVAIL_CURSOR_X_MAX[cursor_y]-1);
					
					awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
					Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
					break;
				case FREQUENCY_CURSOR_Y:
					if(awg_cells[cursor_y][cursor_x].cell_mode == CELL_MODE_NUM)
					{
						Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
					}
					else
					{
						if(awg.Frequency.unit_no == 0)
							awg.Frequency.unit_no = 1;
						else
							awg.Frequency.unit_no = 0;
						awg_cells[cursor_y][cursor_x].text = awg.Frequency.unit_string[awg.Frequency.unit_no];
					}
					Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
					awg.Frequency.actual_value = awg.Frequency.show_value_int[0]+\
																		10*awg.Frequency.show_value_int[1]+\
																	 100*awg.Frequency.show_value_int[2]+\
																	0.1f*awg.Frequency.show_value_dec;
					if(awg.Frequency.unit_no == 1)
						awg.Frequency.actual_value *= 1000;
					break;
				case AMPLITUDE_CURSOR_Y:
					//减1
					Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
					//符号
					if(cursor_x == 0)
					{
						if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
						else
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
					}
					//显示	
					Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
					//修改值
					awg.Amplitude.actual_value = awg.Amplitude.show_value_int[0] + 0.1f*awg.Amplitude.show_value_dec;
					//
					break;
				case OFFSET_CURSOR_Y:
					//减1
					Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
					//符号
					if(cursor_x == 0)
					{
						if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
						else
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
					}
					//显示		
					Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
					//修改值
					awg.Offset.actual_value = awg.Offset.show_value_int[0] + 0.1f*awg.Offset.show_value_dec;
					break;
				default:
					break;
			}
			break;
			
		case KEY_L_PRESSED:
			switch(cursor_y)
			{
				case MODE_CURSOR_Y:
					awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
					Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
					
					cursor_x++;
					resetNum(&cursor_x, 0, AVAIL_CURSOR_X_MAX[cursor_y]-1);
					
					awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
					Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
					break;
				case FREQUENCY_CURSOR_Y:
					if(awg_cells[cursor_y][cursor_x].cell_mode == CELL_MODE_NUM)
					{
						Cell_numIncrease(&(awg_cells[cursor_y][cursor_x]));
					}
					else
					{
						if(awg.Frequency.unit_no == 0)
							awg.Frequency.unit_no = 1;
						else
							awg.Frequency.unit_no = 0;
						awg_cells[cursor_y][cursor_x].text = awg.Frequency.unit_string[awg.Frequency.unit_no];
					}
					Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
					awg.Frequency.actual_value = awg.Frequency.show_value_int[0]+\
																		10*awg.Frequency.show_value_int[1]+\
																	 100*awg.Frequency.show_value_int[2]+\
																	0.1f*awg.Frequency.show_value_dec;
					if(awg.Frequency.unit_no == 1)
						awg.Frequency.actual_value *= 1000;
					break;
				case AMPLITUDE_CURSOR_Y:
					//加1
					Cell_numIncrease(&(awg_cells[cursor_y][cursor_x]));
					//符号
					if(cursor_x == 0)
					{
						if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
						else
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
					}
					//显示	
					Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
					//修改值
					awg.Amplitude.actual_value = awg.Amplitude.show_value_int[0] + 0.1f*awg.Amplitude.show_value_dec;
					//
					break;
				case OFFSET_CURSOR_Y:
					//加1
					Cell_numIncrease(&(awg_cells[cursor_y][cursor_x]));
					//符号
					if(cursor_x == 0)
					{
						if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
						else
							TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
					}
					//显示		
					Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
					//修改值
					awg.Offset.actual_value = awg.Offset.show_value_int[0] + 0.1f*awg.Offset.show_value_dec;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	//输出
	AWG_updateOutput(&awg);
//	if(Key_2.button_flag == 1)
//	{
//		awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
//		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
//		
//		cursor_y++;
//		resetNum(&cursor_y, 0, CURSOR_Y_MAX-1);
//		cursor_x = 0;
//	
//		awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
//		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
//		
//		Key_2.button_flag = 0;
//	}
//	else if(Key_O.button_flag == 1)
//	{
//		awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
//		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
//		
//		cursor_x++;
//		resetNum(&cursor_x, 0, AVAIL_CURSOR_X_MAX[cursor_y]-1);
//		
//		awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
//		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
//		
//		Key_O.button_flag = 0;
//	}
//	else if(Key_L.button_flag == 1)
//	{
//		if(awg_cells[cursor_y][cursor_x].cell_mode == CELL_MODE_NUM)
//		{
//			Cell_numIncrease(&(awg_cells[cursor_y][cursor_x]));
//			Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
//		}
//		switch(cursor_y)
//		{
//			case FREQUENCY_CURSOR_Y:	
//				awg.Frequency.actual_value = awg.Frequency.show_value_int[0]+\
//																	10*awg.Frequency.show_value_int[1]+\
//																 100*awg.Frequency.show_value_int[2]+\
//																0.1f*awg.Frequency.show_value_dec;
//				TIM3->ARR = (100000.0f/awg.Frequency.actual_value) - 1;
//				break;
//			default:
//				break;
//		}
//		
//		Key_L.button_flag = 0;
//	}
//	else if(Key_R.button_flag == 1)
//	{
//		switch(cursor_y)
//		{
//			case FREQUENCY_CURSOR_Y:
//				Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
//				Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
//				awg.Frequency.actual_value = awg.Frequency.show_value_int[0]+\
//																	10*awg.Frequency.show_value_int[1]+\
//																 100*awg.Frequency.show_value_int[2]+\
//																0.1f*awg.Frequency.show_value_dec;
//				TIM3->ARR = (100000.0f/awg.Frequency.actual_value) - 1;
//				break;
//			case AMPLITUDE_CURSOR_Y:
//				//减1
//				Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
//				//符号
//				if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
//					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
//				else
//					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
//				//显示	
//				Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
//				//修改值
//				awg.Amplitude.actual_value = awg.Amplitude.show_value_int[0] + 0.1f*awg.Amplitude.show_value_dec;
//				//
//				break;
//			case OFFSET_CURSOR_Y:
//				//减1
//				Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
//				//符号
//				if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
//					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
//				else
//					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
//				//显示		
//				Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
//				//修改值
//				awg.Offset.actual_value = awg.Offset.show_value_int[0] + 0.1f*awg.Offset.show_value_dec;
//				break;
//			default:
//				break;
//		}
//		
//		Key_R.button_flag = 0;
//	}
}

//void valueIncrease(Value_TypeDef *value)
//{	
//	switch(cursor_x)
//	{
//		case 0:
//			value->show_value_int[3]++;
//			resetNum(&(value->show_value_int[3]),0,9);
//			break;
//		case 1:
//			value->show_value_int[2]++;
//			resetNum(&(value->show_value_int[2]),0,9);
//			break;
//		case 2:
//			value->show_value_int[1]++;
//			resetNum(&(value->show_value_int[1]),0,9);
//			break;
//		case 3:
//			value->show_value_int[0]++;
//			resetNum(&(value->show_value_int[0]),0,9);
//			break;
//		case 4:
//			value->show_value_dec++;
//			resetNum(&(value->show_value_dec),0,9);
//			break;
//		case 5:
//			value->unit_no++;
//			resetNum(&(value->unit_no),0,1);
//			break;
//	}
//}

//void valueDecrease(Value_TypeDef *value)
//{
//	switch(cursor_x)
//	{
//		case 0:
//			value->show_value_int[3]--;
//			resetNum(&(value->show_value_int[3]),0,9);
//			break;
//		case 1:
//			value->show_value_int[2]--;
//			resetNum(&(value->show_value_int[2]),0,9);
//			break;
//		case 2:
//			value->show_value_int[1]--;
//			resetNum(&(value->show_value_int[1]),0,9);
//			break;
//		case 3:
//			value->show_value_int[0]--;
//			resetNum(&(value->show_value_int[0]),0,9);
//			break;
//		case 4:
//			value->show_value_dec--;
//			resetNum(&(value->show_value_dec),0,9);
//			break;
//		case 5:
//			value->unit_no--;
//			resetNum(&(value->unit_no),0,1);
//			break;
//	}
//}
