#include "main.h"

short cursor_x;
short cursor_y;

uint16_t AVAIL_CURSOR_X_MAX[CURSOR_Y_MAX] = {1,3,5,5,3,3};
uint16_t AWG_LCD_Y[CURSOR_Y_MAX] = {45,90,120,150,180,210};

uint16_t AWG_LCD_X[CURSOR_Y_MAX][CURSOR_X_MAX] = {
	{ 15,  0,  0,  0,  0,  0},		//Funtion Generator
	{ 80,130,180,  0,  0,  0},		//Mode:
	{100,108,116,132,140,156},		//Frequency
	{100,108,116,132,140,156},		//Period
	{116,132,140,  0,  0,  0},		//Amplitude
	{116,132,140,  0,  0,  0}			//offset
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
	awg_cells[0][0].cell_mode = CELL_MODE_TXT;
	awg_cells[0][0].text = "Function Generator";
	
	awg_cells[1][0].cell_mode = CELL_MODE_TXT;
	awg_cells[1][0].text = "Sine";
	awg_cells[1][1].cell_mode = CELL_MODE_TXT;
	awg_cells[1][1].text = "Tri";	
	awg_cells[1][2].cell_mode = CELL_MODE_TXT;
	awg_cells[1][2].text = "PWM";
	
	awg_cells[2][0].cell_mode = CELL_MODE_NUM;
	awg_cells[2][0].p_num = &(awg.Frequency.show_value_int[2]);	
	awg_cells[2][0].num_min = 0;
	awg_cells[2][0].num_max = 9;
	awg_cells[2][1].cell_mode = CELL_MODE_NUM;
	awg_cells[2][1].p_num = &(awg.Frequency.show_value_int[1]);	
	awg_cells[2][1].num_min = 0;
	awg_cells[2][1].num_max = 9;
	awg_cells[2][2].cell_mode = CELL_MODE_NUM;
	awg_cells[2][2].p_num = &(awg.Frequency.show_value_int[0]);	
	awg_cells[2][2].num_min = 0;
	awg_cells[2][2].num_max = 9;
	awg_cells[2][3].cell_mode = CELL_MODE_NUM;
	awg_cells[2][3].p_num = &(awg.Frequency.show_value_dec);
	awg_cells[2][3].num_min = 0;
	awg_cells[2][3].num_max = 9;
	awg_cells[2][4].cell_mode = CELL_MODE_TXT;
	awg_cells[2][4].text = awg.Frequency.unit_string[awg.Frequency.unit_no];
	
	awg_cells[4][0].cell_mode = CELL_MODE_NUM;
	awg_cells[4][0].p_num = &(awg.Amplitude.show_value_int[0]);
	awg_cells[4][0].num_min = -4;
	awg_cells[4][0].num_max = 4;
	awg_cells[4][1].cell_mode = CELL_MODE_NUM;
	awg_cells[4][1].p_num = &(awg.Amplitude.show_value_dec);
	awg_cells[4][1].num_min = 0;
	awg_cells[4][1].num_max = 9;
	awg_cells[4][2].cell_mode = CELL_MODE_TXT;
	awg_cells[4][2].text = awg.Amplitude.unit_string[awg.Frequency.unit_no];
	
	awg_cells[5][0].cell_mode = CELL_MODE_NUM;
	awg_cells[5][0].p_num = &(awg.Offset.show_value_int[0]);
	awg_cells[5][0].num_min = -4;
	awg_cells[5][0].num_max = 4;
	awg_cells[5][1].cell_mode = CELL_MODE_NUM;
	awg_cells[5][1].p_num = &(awg.Offset.show_value_dec);
	awg_cells[5][1].num_min = 0;
	awg_cells[5][1].num_max = 9;
	awg_cells[5][2].cell_mode = CELL_MODE_TXT;
	awg_cells[5][2].text = awg.Offset.unit_string[awg.Frequency.unit_no];
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
	
	TFT_ShowString(15,150,"Period:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(124,150,'.',8,16,0,BLACK,WHITE);
	
	TFT_ShowString(15,180,"Amplitude:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(124,180,'.',8,16,0,BLACK,WHITE);
	
	TFT_ShowString(15,210,"Offset:",8,16,0,BLACK,WHITE);
	TFT_ShowChar(124,210,'.',8,16,0,BLACK,WHITE);
}

void interfaceUpdate(void)
{
	if(Key_2.button_flag == 1)
	{
		awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
		
		cursor_y++;
		resetNum(&cursor_y, 0, CURSOR_Y_MAX-1);
		cursor_x = 0;
	
		awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
		
		Key_2.button_flag = 0;
	}
	else if(Key_O.button_flag == 1)
	{
		awg_cells[cursor_y][cursor_x].is_selected = NOT_SELECTED;
		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
		
		cursor_x++;
		resetNum(&cursor_x, 0, AVAIL_CURSOR_X_MAX[cursor_y]-1);
		
		awg_cells[cursor_y][cursor_x].is_selected = IS_SELECTED;
		Cell_TFT_Show(&awg_cells[cursor_y][cursor_x]);
		
		Key_O.button_flag = 0;
	}
	else if(Key_L.button_flag == 1)
	{
		if(awg_cells[cursor_y][cursor_x].cell_mode == CELL_MODE_NUM)
		{
			Cell_numIncrease(&(awg_cells[cursor_y][cursor_x]));
			Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
		}
		switch(cursor_y)
		{
			case FREQUENCY_CURSOR_Y:	
				awg.Frequency.actual_value = awg.Frequency.show_value_int[0]+\
																	10*awg.Frequency.show_value_int[1]+\
																 100*awg.Frequency.show_value_int[2]+\
																0.1f*awg.Frequency.show_value_dec;
				TIM3->ARR = (100000.0f/awg.Frequency.actual_value) - 1;
				break;
			default:
				break;
		}
		
		Key_L.button_flag = 0;
	}
	else if(Key_R.button_flag == 1)
	{
		switch(cursor_y)
		{
			case FREQUENCY_CURSOR_Y:
				Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
				Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
				awg.Frequency.actual_value = awg.Frequency.show_value_int[0]+\
																	10*awg.Frequency.show_value_int[1]+\
																 100*awg.Frequency.show_value_int[2]+\
																0.1f*awg.Frequency.show_value_dec;
				TIM3->ARR = (100000.0f/awg.Frequency.actual_value) - 1;
				break;
			case AMPLITUDE_CURSOR_Y:
				Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
				if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
				else
					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
					
				Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
				break;
			case OFFSET_CURSOR_Y:
				Cell_numDecrease(&(awg_cells[cursor_y][cursor_x]));
				if(*(awg_cells[cursor_y][cursor_x].p_num) < 0)
					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, '-', FONT_W, FONT_H, 1, BLACK, WHITE);
				else
					TFT_ShowChar(awg_cells[cursor_y][cursor_x].LCD_X - 8, awg_cells[cursor_y][cursor_x].LCD_Y, ' ', FONT_W, FONT_H, 1, BLACK, WHITE);
					
				Cell_TFT_Show(&(awg_cells[cursor_y][cursor_x]));
				break;
			default:
				break;
		}
		
		Key_R.button_flag = 0;
	}
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
