#include "main.h"
#include "gpio.h"
#include "spi.h"

uint16_t BACK_COLOR=BLACK;
uint16_t POINT_COLOR=WHITE;

/**
 * @brief    SPI �����ֽں���
 * @param    TxData	Ҫ���͵�����
 * @param    size	�������ݵ��ֽڴ�С
 * @return  0:д��ɹ�,����:д��ʧ��
 */
uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size)
{
	return HAL_SPI_Transmit(&hspi1,TxData,size,1000);
}

/**
 *@brief    LCD�������ź�ͨ�Žӿڳ�ʼ��
 *@param    none
 *@retval   none
*/
static void LCD_GPIO_Init(void)
{
    /* ��ʼ������ */
	MX_GPIO_Init();
		
	/* ��λLCD */
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1);

	/* ��ʼ��SPI2�ӿ� */
    MX_SPI1_Init();
}

/**
 * @brief   д���LCD
 * @param   cmd ���� ��Ҫ���͵�����
 * @return  none
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_WR_RS(0);
    SPI_WriteByte(&cmd, 1);
}

/**
 * @brief   д���ݵ�LCD
 * @param   dat ���� ��Ҫ���͵�����
 * @return  none
 */
static void LCD_Write_Data(uint8_t dat)
{
    LCD_WR_RS(1);
    SPI_WriteByte(&dat, 1);
}


//TFTдһ���ֽ�����
//dat��д����ֽ�
//cmd:
//	0������
//	1������	
static void TFT_WriteByte(u8 dat,u8 cmd)
{
	if(cmd == TFT_CMD)	
		LCD_WR_RS(0);   //����
  else 
		LCD_WR_RS(1);		//���� 
  
   SPI_WriteByte(&dat, 1);
}

/**
 * @brief   ��������д��LCD�Դ�����
 * @param   x1,y1	���� �������
 * @param   x2,y2	���� �յ�����
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* ָ��X����������� */
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    /* ָ��Y����������� */
    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    /* ���͸����LCD��ʼ�ȴ������Դ����� */
    LCD_Write_Cmd(0x2C);
}

#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152
static uint8_t lcd_buf[LCD_Buf_Size];
/**
 * @brief   ��һ����ɫ���LCD��
 * @param   color ���� ������ɫ(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};  //color��16bit�ģ�ÿ�����ص���Ҫ�����ֽڵ��Դ�

    /* ��16bit��colorֵ�ֿ�Ϊ�����������ֽ� */
    data[0] = color >> 8;
    data[1] = color;
    
    /* �Դ��ֵ��Ҫ���ֽ�д�� */
    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }
    /* ָ���Դ������ַΪȫ��Ļ */
    LCD_Address_Set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    /* ָ��������������Ϊ���� */
    LCD_WR_RS(1);
    /* ���Դ滺����������ȫ��д�뻺���� */
    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        SPI_WriteByte(lcd_buf, (uint16_t)LCD_Buf_Size);
    }
}

/**
 * @brief   LCD��ʼ��
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
    /* ��ʼ����LCDͨ�ŵ����� */
    LCD_GPIO_Init();
    HAL_Delay(120);
	
    /* �ر�˯��ģʽ */
    LCD_Write_Cmd(0x11);
    HAL_Delay(120);

    /* ��ʼ�����Դ�ɨ��ģʽ�����ݸ�ʽ�� */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);
    /* RGB 5-6-5-bit��ʽ  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);
    /* porch ���� */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);
    /* VGH���� */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);
    /* VCOM ���� */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);
    /* LCM ���� */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);
    /* VDV and VRH ���� */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);
    /* VRH ���� */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);
    /* VDV ���� */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);
    /* ��ͨģʽ���Դ��������� 60Mhz */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);
    /* ��Դ���� */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);
    /* ��ѹ���� */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);
    /* ��ѹ���� */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);
    /* ��ʾ�� */
    LCD_Write_Cmd(0x21);
    LCD_Write_Cmd(0x29);
    
    /* ����Ϊ��ɫ */
    LCD_Clear(WHITE);
}

//����
//x��y:��ʼ����
//color:��ɫ
void TFT_DrawPoint(u16 x,u16 y,u16 color)
{
    LCD_Address_Set(x,y,x,y);//���ù��λ��
    TFT_WriteByte(color>>8,TFT_DATA);
    TFT_WriteByte(color,TFT_DATA);
}

//�������
//x1��y1:��ʼ����
//x2��y2:��������
//color:��ɫ
void TFT_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
    u16 x,y;
    LCD_Address_Set(x1,y1,x2,y2);      //���ù��λ��
	
    for(x=x1; x<x2; x++){
        for(y=y1; y<y2; y++){
			TFT_WriteByte(color>>8,TFT_DATA); //���ù��λ��
			TFT_WriteByte(color,TFT_DATA); //���ù��λ��
		}
    }
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//����˵������ʾ�ַ�
//������ݣ�x,y    �������
//		chr    Ҫ��ʾ���ַ�
//		mode   1���ӷ�ʽ  0�ǵ��ӷ�ʽ
void TFT_ShowChar(u16 x,u16 y,u8 chr,u8 fw,u8 fh,u8 mode,u16 back_color, u16 point_color)
{
    u8 temp, t, tbit;
    u8 y0=y;
    u8 *p;	
	
	u16 csize = csize=(fh/8+((fh%8)?1:0))*fw;// �õ�����һ���ַ���Ӧ������ռ���ֽ���	
 
    chr=chr-' ';//�õ�ƫ�ƺ��ֵ
 
    if(fw==6&&fh==8)        p = (u8 *)asc2_0608[chr];	//����0608ascii����
    else if(fw==6&&fh==12)  p = (u8 *)asc2_0612[chr];	//����0612ascii����
    else if(fw==8&&fh==16)  p = (u8 *)asc2_0816[chr];	//����0612ascii����
    else if(fw==12&&fh==24) p = (u8 *)asc2_1224[chr];   //����1224ascii����
    else return;	//û�е��ֿ�
	for(t = 0; t < csize; t++)	/*������ӡ�������ص㵽LCD */
	{   
		temp = p[t];
		for(tbit = 0; tbit < 8; tbit++)	/* ��ӡһ�����ص㵽Һ�� */
		{	
			u16 color;
			
			if(temp & 0x80)	color = point_color;
			else if(0 == mode)	color = back_color;
			else color = ~point_color;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			y++;
			
			if(y >= LCD_HEIGHT) return;		/* �������� */
 
			if((y - y0) == fh){
				y = y0;
				x++;
				if(x >= LCD_WIDTH)	return;	/* �������� */
				break;
			}
		}  	 
	}  	  
}
//m^n����
u32 mypow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//��ʾ����
//x,y :�������
//len :���ֵ�λ��
//fw:�ֿ�
//fh:�ָ�
//num:��ֵ(0~4294967295);
void TFT_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 fw,u8 fh, u16 back_color, u16 point_color)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                TFT_ShowChar(x+(fw)*t,y,' ',fw,fh,0,back_color, point_color);
                continue;
            }
            else
                enshow=1;
        }
        TFT_ShowChar(x+(fw)*t,y,temp+'0',fw,fh,0,back_color, point_color);
    }
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//fw:�ֿ�
//fh:�ָ�
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ
void TFT_ShowString(u16 x,u16 y,char *str,u8 fw,u8 fh,u8 mode,u16 back_color, u16 point_color)
{
    while((*str<='~')&&(*str>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x>(LCD_WIDTH-(fw))) {
            x=0;
            y+=fh;
        }
        if(y>(LCD_HEIGHT-fh)) {
            y=x=0;   //TFT_Clear();
        }
        TFT_ShowChar(x,y,*str,fw,fh,mode,back_color, point_color);
        x+=fw;
        str++;
    }
}


