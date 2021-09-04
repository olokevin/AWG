#ifndef __LCD_DRV_H
#define __LCD_DRV_H

/*
x������
y������
(0,0)�������Ͻ�
*/

#include "main.h"

#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t

#define TFT_CMD 	0
#define TFT_DATA	1

//LCD�������Ŷ���
#define	LCD_WR_RS(n)	(n?\
						HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))
#define	LCD_DC(n)			(n?\
						HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?\
						HAL_GPIO_WritePin(LCD_RSTn_GPIO_Port,LCD_RSTn_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_RSTn_GPIO_Port,LCD_RSTn_Pin,GPIO_PIN_RESET))

//LCD��Ļ�ֱ��ʶ���
#define LCD_WIDTH   240
#define LCD_HEIGHT  240

//��ɫ����
#define WHITE   0xFFFF	//��ɫ
#define YELLOW  0xFFE0	//��ɫ
#define BRRED   0XFC07  //�غ�ɫ
#define PINK    0XF81F	//��ɫ
#define RED     0xF800	//��ɫ
#define BROWN   0XBC40  //��ɫ
#define GRAY    0X8430  //��ɫ
#define GBLUE   0X07FF	//��ɫ
#define GREEN   0x07E0	//��ɫ
#define BLUE    0x001F  //��ɫ
#define BLACK   0x0000	//��ɫ

uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size);
static void LCD_GPIO_Init(void);
static void LCD_Write_Cmd(uint8_t cmd);
static void LCD_Write_Data(uint8_t dat);
static void TFT_WriteByte(u8 dat,u8 cmd);

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Clear(uint16_t color);
void LCD_Init(void);
void TFT_DrawPoint(u16 x,u16 y,u16 color);
void TFT_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);

/*
��ģ��0608	0612	0816	1224 
0608��06ָһ���ַ�/����x����ռ6�����أ�08ָy����ռ8������
��ʾ���֣�����ʵ�ָ��ǣ�ÿ�ζ�ռ4��λ�ã�ʵ�����3λ����һλҲΪ�ո�
*/
void TFT_ShowChar(u16 x,u16 y,u8 chr,u8 fw,u8 fh,u8 mode,u16 back_color, u16 point_color);
u32 mypow(u8 m,u8 n);
void TFT_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 fw,u8 fh,u16 back_color, u16 point_color);
void TFT_ShowString(u16 x,u16 y,char *str,u8 fw,u8 fh,u8 mode,u16 back_color, u16 point_color);

#endif /* __MAIN_H */
