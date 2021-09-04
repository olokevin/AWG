#include "main.h"

uint16_t sineOrigin[48] = {
2051,1851,1652,1457,1268,1086,913,752,603,468,348,244,158,90,40,10,0,
9,38,86,153,239,341,460,595,743,904,1076,1257,1446,1641,1839,2040,2241,
2440,2635,2824,3007,3180,3342,3492,3628,3748,3853,3940,4010,4060,4091
};

AWG_TypeDef awg;

void AWG_Init(void)
{
	awg.Cursor	= AWG_CURSOR_NONE;
	awg.Mode		= AWG_MODE_SINE;
	
	awg.Frequency.actual_value = 100;
	awg.Frequency.show_value_int[2] = 1;
	awg.Frequency.show_value_int[1] = 0;
	awg.Frequency.show_value_int[0] = 0;
	awg.Frequency.show_value_dec = 0;
	awg.Frequency.unit_no = 0;
	awg.Frequency.unit_string[0] = "Hz";
	awg.Frequency.unit_string[1] = "kHz";
}

