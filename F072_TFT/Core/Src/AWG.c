#include "main.h"

uint16_t sine_data[SAMPLE_POINTS] = {
2048,2001,1954,1908,1861,1814,1768,1722,1676,1630,1584,1539,1494,1450,
1406,1362,1319,1277,1235,1193,1152,1112,1072,1033,995,957,920,884,849,
	815,781,748,716,685,655,626,598,571,545,520,496,473,451,431,411,392,
	375,359,344,330,317,306,295,286,278,271,266,262,258,257,256,257,258,
	262,266,271,278,286,295,306,317,330,344,359,375,392,411,431,451,473,
	496,520,545,571,598,626,655,685,716,748,781,815,849,884,920,957,995,
	1033,1072,1112,1152,1193,1235,1277,1319,1362,1406,1450,1494,1539,
	1584,1630,1676,1722,1768,1814,1861,1908,1954,2001,2048,2095,2142,
	2189,2236,2282,2329,2375,2421,2467,2512,2557,2602,2646,2690,2734,
	2777,2820,2862,2903,2944,2985,3024,3063,3102,3139,3176,3212,3247,
	3282,3315,3348,3380,3411,3441,3470,3498,3525,3551,3576,3600,3623,
	3645,3666,3685,3704,3721,3738,3753,3767,3779,3791,3801,3810,3818,
	3825,3831,3835,3838,3840,3840,3840,3838,3835,3831,3825,3818,3810,
	3801,3791,3779,3767,3753,3738,3721,3704,3685,3666,3645,3623,3600,
	3576,3551,3525,3498,3470,3441,3411,3380,3348,3315,3282,3247,3212,
	3176,3139,3102,3063,3024,2985,2944,2903,2862,2820,2777,2734,2690,
	2646,2602,2557,2512,2467,2421,2375,2329,2282,2236,2189,2142,2095
};

uint16_t tri_data[SAMPLE_POINTS] = {
	2033,2018,2003,1988,1974,1959,1944,1929,1914,1899,1884,1869,1854,
	1839,1824,1809,1794,1779,1764,1750,1735,1720,1705,1690,1675,1660,
	1645,1630,1615,1600,1585,1570,1555,1540,1525,1511,1496,1481,1466,
	1451,1436,1421,1406,1391,1376,1361,1346,1331,1316,1301,1287,1272,
	1257,1242,1227,1212,1197,1182,1167,1152,1137,1122,1107,1092,1077,
	1063,1048,1033,1018,1003,988,973,958,943,928,913,898,883,868,853,
	838,824,809,794,779,764,749,734,719,704,689,674,659,644,629,614,600,
	585,570,555,540,525,510,495,480,465,450,435,420,405,390,376,361,346,
	331,316,301,286,271,256,271,286,301,316,331,346,361,376,390,405,420,
	435,450,465,480,495,510,525,540,555,570,585,600,614,629,644,659,674,
	689,704,719,734,749,764,779,794,809,824,838,853,868,883,898,913,928,
	943,958,973,988,1003,1018,1033,1048,1063,1077,1092,1107,1122,1137,
	1152,1167,1182,1197,1212,1227,1242,1257,1272,1287,1301,1316,1331,
	1346,1361,1376,1391,1406,1421,1436,1451,1466,1481,1496,1511,1525,
	1540,1555,1570,1585,1600,1615,1630,1645,1660,1675,1690,1705,1720,
	1735,1750,1764,1779,1794,1809,1824,1839,1854,1869,1884,1899,1914,
	1929,1944,1959,1974,1988,2003,2018,2033,2048
};

uint16_t dac_output[SAMPLE_POINTS];

AWG_TypeDef awg;
uint16_t AVAIL_CURSOR_X_MAX[CURSOR_Y_MAX] = {3,5,3,3};
uint16_t AWG_LCD_Y[CURSOR_Y_MAX] = {90,120,150,180};

uint16_t AWG_LCD_X[CURSOR_Y_MAX][CURSOR_X_MAX] = {
	{ 80,130,180,  0,  0,  0},		//Mode:
	{100,108,116,132,140,156},		//Frequency
	{116,132,140,  0,  0,  0},		//Amplitude
	{116,132,140,  0,  0,  0}		//Offset
};

Cell_Typedef awg_cells[CURSOR_Y_MAX][CURSOR_X_MAX];

void AWG_dataInit(void)
{
	awg.Cursor	= AWG_CURSOR_NONE;
	awg.Mode		= AWG_MODE_SINE;
	
	awg.Frequency.actual_value = 100;
	awg.Frequency.show_value_int[2] = 1;
	awg.Frequency.show_value_int[1] = 0;
	awg.Frequency.show_value_int[0] = 0;
	awg.Frequency.show_value_dec = 0;
	awg.Frequency.unit_no = 0;
	awg.Frequency.unit_string[0] = "Hz ";
	awg.Frequency.unit_string[1] = "kHz";
	
	awg.Amplitude.actual_value = 4.0f;
	awg.Amplitude.show_value_int[0] = 4;
	awg.Amplitude.show_value_dec = 0;
	awg.Amplitude.unit_no = 0;
	awg.Amplitude.unit_string[0] = "V";
	
	awg.Offset.actual_value = 0;
	awg.Offset.show_value_int[0] = 0;
	awg.Offset.show_value_dec = 0;
	awg.Offset.unit_no = 0;
	awg.Offset.unit_string[0] = "V";
}

void AWG_cellInit(void)
{
	//Contents init	
	awg_cells[MODE_CURSOR_Y][0].cell_mode = CELL_MODE_TXT;
	awg_cells[MODE_CURSOR_Y][0].text = "Sine";
	awg_cells[MODE_CURSOR_Y][0].is_selected = IS_SELECTED;
	awg_cells[MODE_CURSOR_Y][1].cell_mode = CELL_MODE_TXT;
	awg_cells[MODE_CURSOR_Y][1].text = "Tri";	
	awg_cells[MODE_CURSOR_Y][2].cell_mode = CELL_MODE_TXT;
	awg_cells[MODE_CURSOR_Y][2].text = "SQR";
	
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

void AWG_interfaceUpdate(void)
{
	switch(button_state)
	{
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
}

void AWG_dacUpdate(AWG_TypeDef *p_awg, uint16_t* origin_array, uint16_t* output_array)
{
	int i;
	uint16_t tim3_period;
	
	//Stop Output
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);	
	HAL_TIM_Base_Stop(&htim3);
	
	// Set TIM3 autoreload.
	tim3_period = (100000.0f/p_awg->Frequency.actual_value) - 1;
	TIM3->CNT = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim3, tim3_period);

	// Set Output
	for(i=0;i<SAMPLE_POINTS;i++)
		output_array[i] = (uint16_t)((origin_array[i] + p_awg->Offset.actual_value) * p_awg->Amplitude.actual_value / 4.0f);
	
	//Start Output
  HAL_TIM_Base_Start(&htim3);
	HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)output_array,SAMPLE_POINTS,DAC_ALIGN_12B_R);	
}

void AWG_updateOutput(AWG_TypeDef *p_awg)
{
	switch(p_awg->Mode)
	{
		case AWG_MODE_SINE:
			AWG_dacUpdate(p_awg, sine_data, dac_output);
			break;
		case AWG_MODE_TRI:
			AWG_dacUpdate(p_awg, tri_data, dac_output);
			break;
		case AWG_MODE_SQR:
			break;
		default:
			break;
	}	
}

