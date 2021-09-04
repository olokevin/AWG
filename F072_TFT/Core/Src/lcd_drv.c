#include "main.h"
#include "gpio.h"
#include "spi.h"

uint16_t BACK_COLOR=BLACK;
uint16_t POINT_COLOR=WHITE;

/**
 * @brief    SPI 发送字节函数
 * @param    TxData	要发送的数据
 * @param    size	发送数据的字节大小
 * @return  0:写入成功,其他:写入失败
 */
uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size)
{
	return HAL_SPI_Transmit(&hspi1,TxData,size,1000);
}

/**
 *@brief    LCD控制引脚和通信接口初始化
 *@param    none
 *@retval   none
*/
static void LCD_GPIO_Init(void)
{
    /* 初始化引脚 */
	MX_GPIO_Init();
		
	/* 复位LCD */
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1);

	/* 初始化SPI2接口 */
    MX_SPI1_Init();
}

/**
 * @brief   写命令到LCD
 * @param   cmd —— 需要发送的命令
 * @return  none
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_WR_RS(0);
    SPI_WriteByte(&cmd, 1);
}

/**
 * @brief   写数据到LCD
 * @param   dat —— 需要发送的数据
 * @return  none
 */
static void LCD_Write_Data(uint8_t dat)
{
    LCD_WR_RS(1);
    SPI_WriteByte(&dat, 1);
}


//TFT写一个字节数据
//dat：写入的字节
//cmd:
//	0：命令
//	1：数据	
static void TFT_WriteByte(u8 dat,u8 cmd)
{
	if(cmd == TFT_CMD)	
		LCD_WR_RS(0);   //命令
  else 
		LCD_WR_RS(1);		//数据 
  
   SPI_WriteByte(&dat, 1);
}

/**
 * @brief   设置数据写入LCD显存区域
 * @param   x1,y1	—— 起点坐标
 * @param   x2,y2	—— 终点坐标
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* 指定X方向操作区域 */
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    /* 指定Y方向操作区域 */
    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    /* 发送该命令，LCD开始等待接收显存数据 */
    LCD_Write_Cmd(0x2C);
}

#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152
static uint8_t lcd_buf[LCD_Buf_Size];
/**
 * @brief   以一种颜色清空LCD屏
 * @param   color —— 清屏颜色(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};  //color是16bit的，每个像素点需要两个字节的显存

    /* 将16bit的color值分开为两个单独的字节 */
    data[0] = color >> 8;
    data[1] = color;
    
    /* 显存的值需要逐字节写入 */
    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }
    /* 指定显存操作地址为全屏幕 */
    LCD_Address_Set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    /* 指定接下来的数据为数据 */
    LCD_WR_RS(1);
    /* 将显存缓冲区的数据全部写入缓冲区 */
    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        SPI_WriteByte(lcd_buf, (uint16_t)LCD_Buf_Size);
    }
}

/**
 * @brief   LCD初始化
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
    /* 初始化和LCD通信的引脚 */
    LCD_GPIO_Init();
    HAL_Delay(120);
	
    /* 关闭睡眠模式 */
    LCD_Write_Cmd(0x11);
    HAL_Delay(120);

    /* 开始设置显存扫描模式，数据格式等 */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);
    /* RGB 5-6-5-bit格式  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);
    /* porch 设置 */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);
    /* VGH设置 */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);
    /* VCOM 设置 */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);
    /* LCM 设置 */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);
    /* VDV and VRH 设置 */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);
    /* VRH 设置 */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);
    /* VDV 设置 */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);
    /* 普通模式下显存速率设置 60Mhz */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);
    /* 电源控制 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);
    /* 电压设置 */
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
    /* 电压设置 */
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
    /* 显示开 */
    LCD_Write_Cmd(0x21);
    LCD_Write_Cmd(0x29);
    
    /* 清屏为白色 */
    LCD_Clear(WHITE);
}

//画点
//x，y:起始坐标
//color:颜色
void TFT_DrawPoint(u16 x,u16 y,u16 color)
{
    LCD_Address_Set(x,y,x,y);//设置光标位置
    TFT_WriteByte(color>>8,TFT_DATA);
    TFT_WriteByte(color,TFT_DATA);
}

//填充区域
//x1，y1:起始坐标
//x2，y2:结束坐标
//color:颜色
void TFT_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
    u16 x,y;
    LCD_Address_Set(x1,y1,x2,y2);      //设置光标位置
	
    for(x=x1; x<x2; x++){
        for(y=y1; y<y2; y++){
			TFT_WriteByte(color>>8,TFT_DATA); //设置光标位置
			TFT_WriteByte(color,TFT_DATA); //设置光标位置
		}
    }
}


//在指定位置显示一个字符,包括部分字符
//函数说明：显示字符
//入口数据：x,y    起点坐标
//		chr    要显示的字符
//		mode   1叠加方式  0非叠加方式
void TFT_ShowChar(u16 x,u16 y,u8 chr,u8 fw,u8 fh,u8 mode,u16 back_color, u16 point_color)
{
    u8 temp, t, tbit;
    u8 y0=y;
    u8 *p;	
	
	u16 csize = csize=(fh/8+((fh%8)?1:0))*fw;// 得到字体一个字符对应点阵集所占的字节数	
 
    chr=chr-' ';//得到偏移后的值
 
    if(fw==6&&fh==8)        p = (u8 *)asc2_0608[chr];	//调用0608ascii字体
    else if(fw==6&&fh==12)  p = (u8 *)asc2_0612[chr];	//调用0612ascii字体
    else if(fw==8&&fh==16)  p = (u8 *)asc2_0816[chr];	//调用0612ascii字体
    else if(fw==12&&fh==24) p = (u8 *)asc2_1224[chr];   //调用1224ascii字体
    else return;	//没有的字库
	for(t = 0; t < csize; t++)	/*遍历打印所有像素点到LCD */
	{   
		temp = p[t];
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			u16 color;
			
			if(temp & 0x80)	color = point_color;
			else if(0 == mode)	color = back_color;
			else color = ~point_color;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			y++;
			
			if(y >= LCD_HEIGHT) return;		/* 超区域了 */
 
			if((y - y0) == fh){
				y = y0;
				x++;
				if(x >= LCD_WIDTH)	return;	/* 超区域了 */
				break;
			}
		}  	 
	}  	  
}
//m^n函数
u32 mypow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//显示数字
//x,y :起点坐标
//len :数字的位数
//fw:字宽
//fh:字高
//num:数值(0~4294967295);
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
//在指定位置开始显示一个字符串
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//fw:字宽
//fh:字高
//mode:0,非叠加方式;1,叠加方式
void TFT_ShowString(u16 x,u16 y,char *str,u8 fw,u8 fh,u8 mode,u16 back_color, u16 point_color)
{
    while((*str<='~')&&(*str>=' '))//判断是不是非法字符!
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


