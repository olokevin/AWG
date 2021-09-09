#include "main.h"

short cursor_x;
short cursor_y;

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


