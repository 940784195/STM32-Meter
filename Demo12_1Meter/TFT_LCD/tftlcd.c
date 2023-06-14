#include "tftlcd.h"
//#include "SysTick.h"
//#include "usart.h"
#include "font.h"
//作者王维波


#include	<stdio.h>
#include	<math.h>
#include	<string.h>

#ifdef USE_HZ_LIB
	#include "HzLib.h"
#endif


//LCD的画笔颜色和背景色	   
uint16_t 	LcdFRONT_COLOR=lcdColor_YELLOW;	//画笔颜色,lcdColor_WHITE
uint16_t 	LcdBACK_COLOR=lcdColor_BLACK;  //背景色

TFTLCD_ParaDef LcdPara;

uint16_t LCD_W;		//LCD 宽度,会在LCD_Display_Dir()函数里初始化
uint16_t LCD_H;		//LCD 高度

uint16_t LCD_CurX=0;	//当前位置X
uint16_t LCD_CurY=0;	//当前位置Y

#ifdef TFTLCD_HX8352C		//3.6寸电阻屏,分辨率400*240
	uint8_t		LCD_FS=12;	//默认字体大小

	uint16_t	LCD_SP10=15;	//1.0倍行距
	uint16_t	LCD_SP15=22;	//1.5倍行距
	uint16_t	LCD_SP20=30;	//2.0倍行距
#endif

#ifdef TFTLCD_ILI9486		//3.5寸电阻屏的驱动芯片,480*320
	uint8_t		LCD_FS=16;	//默认字体大小

	uint16_t	LCD_SP10=20;	//1.0倍行距
	uint16_t	LCD_SP15=30;	//1.5倍行距
	uint16_t	LCD_SP20=40;	//2.0倍行距
#endif

#ifdef TFTLCD_ILI9481		//3.5寸电阻屏的驱动芯片,480*320
	uint8_t		LCD_FS=16;	//默认字体大小

	uint16_t	LCD_SP10=20;	//1.0倍行距
	uint16_t	LCD_SP15=30;	//1.5倍行距
	uint16_t	LCD_SP20=40;	//2.0倍行距
#endif

#ifdef TFTLCD_ILI9488		//3.5寸电阻屏的驱动芯片,480*320
	uint8_t		LCD_FS=16;	//默认字体大小

	uint16_t	LCD_SP10=20;	//1.0倍行距
	uint16_t	LCD_SP15=30;	//1.5倍行距
	uint16_t	LCD_SP20=40;	//2.0倍行距
#endif

#ifdef	TFTLCD_NT35510		//4.3寸电容屏，800*480
	uint8_t		LCD_FS=24;	//默认字体大小

	uint16_t	LCD_SP10=30;	//1.0倍行距
	uint16_t	LCD_SP15=40;	//1.5倍行距
	uint16_t	LCD_SP20=50;	//2.0倍行距

#endif

//void TFTLCD_GPIO_Init(void)
//{//这是使用标准库时，LCD 接口引脚的初始化，不用了
//	GPIO_InitTypeDef  GPIO_InitStructure;
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
//							RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能PD,PE,PF,PG时钟
//
//
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);  //FSMC_D0-FSMC_D15
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
//
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);  //FSMC_NOE
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);  //FSMC_NWE
//	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//FSMC_A6
//	GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);//FSMC_NE4
//
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4
//	                              | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9
//								  | GPIO_Pin_10 | GPIO_Pin_14| GPIO_Pin_15;//PD0,1,4,5,8,9,10,14,15
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
//	                              | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
//								  | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;//PE7~15
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PG12,FSMC_NE4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

//}


//void TFTLCD_FSMC_Init(void)
//{
//	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
//	FSMC_NORSRAMTimingInitTypeDef  FSMC_ReadNORSRAMTiming;
//	FSMC_NORSRAMTimingInitTypeDef  FSMC_WriteNORSRAMTiming;
//
//	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟
//
//
//	FSMC_ReadNORSRAMTiming.FSMC_AddressSetupTime = 0X01;	 //地址建立时间（ADDSET）为2个HCLK 1/168M=6ns*2=12ns
//	FSMC_ReadNORSRAMTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到
//	FSMC_ReadNORSRAMTiming.FSMC_DataSetupTime = 0x0f;			//数据保存时间为16个HCLK	因为液晶驱动IC的读数据的时候，速度不能太快
//	FSMC_ReadNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
//	FSMC_ReadNORSRAMTiming.FSMC_CLKDivision = 0x00;
//	FSMC_ReadNORSRAMTiming.FSMC_DataLatency = 0x00;
//	FSMC_ReadNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A
//
//#ifdef defined(TFTLCD_NT35510)|| defined(TFTLCD_ILI9488)
//	FSMC_WriteNORSRAMTiming.FSMC_AddressSetupTime =0x03;	      //地址建立时间（ADDSET）为1个HCLK
//	FSMC_WriteNORSRAMTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A
//	FSMC_WriteNORSRAMTiming.FSMC_DataSetupTime = 0x02;		 //数据保存时间为6ns*9个HCLK=54ns
//	FSMC_WriteNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
//	FSMC_WriteNORSRAMTiming.FSMC_CLKDivision = 0x00;
//	FSMC_WriteNORSRAMTiming.FSMC_DataLatency = 0x00;
//	FSMC_WriteNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A
//#endif
//
//#if	defined(TFTLCD_HX8357D)||defined(TFTLCD_HX8352C)||defined(TFTLCD_ST7793)||
//	defined(TFTLCD_ILI9327)||defined(TFTLCD_ILI9486)
//
//	FSMC_WriteNORSRAMTiming.FSMC_AddressSetupTime =0x10;	      //地址建立时间（ADDSET）为1个HCLK
//	FSMC_WriteNORSRAMTiming.FSMC_AddressHoldTime = 0x10;	 //地址保持时间（A
//	FSMC_WriteNORSRAMTiming.FSMC_DataSetupTime = 0x18;		 //数据保存时间为6ns*9个HCLK=54ns
//	FSMC_WriteNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
//	FSMC_WriteNORSRAMTiming.FSMC_CLKDivision = 0x00;
//	FSMC_WriteNORSRAMTiming.FSMC_DataLatency = 0x00;
//	FSMC_WriteNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A
//#endif
//
//	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
//	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
//	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM
//	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit
//	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
//	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
//	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
//	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadNORSRAMTiming; //读写时序
//	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_WriteNORSRAMTiming;  //写时序
//
//	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置
//
//	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1
//
//}

//写寄存器函数
//cmd:寄存器值
void LCD_WriteCmd(uint16_t cmd)
{
#ifdef TFTLCD_HX8357D	
	TFTLCD->LCD_CMD=cmd;//写入要写的寄存器序号
#endif

#ifdef TFTLCD_HX8352C
	TFTLCD->LCD_CMD=cmd<<8;
#endif

#ifdef TFTLCD_ILI9488
	TFTLCD->LCD_CMD=cmd;
#endif

#ifdef TFTLCD_ILI9327
	TFTLCD->LCD_CMD=cmd;
#endif

#ifdef TFTLCD_ST7793		
	TFTLCD->LCD_CMD=cmd>>8;
	TFTLCD->LCD_CMD=cmd&0xff;
#endif
	
#ifdef TFTLCD_NT35510		
	TFTLCD->LCD_CMD=cmd;	
#endif

#ifdef TFTLCD_ILI9486
	TFTLCD->LCD_CMD=cmd;
#endif

#ifdef TFTLCD_ILI9481
	TFTLCD->LCD_CMD=cmd;
#endif

}

//写数据
//data:要写入的值
void LCD_WriteData(uint16_t data)
{
#ifdef TFTLCD_HX8357D	
	TFTLCD->LCD_DATA=data;//写入要写的寄存器序号
#endif
	
#ifdef TFTLCD_HX8352C
	TFTLCD->LCD_DATA=data<<8;
#endif

#ifdef TFTLCD_ILI9488
	TFTLCD->LCD_DATA=data;
#endif
	
#ifdef TFTLCD_ILI9327
	TFTLCD->LCD_DATA=data;
#endif
	
#ifdef TFTLCD_ST7793	
	TFTLCD->LCD_DATA=data>>8;	
	TFTLCD->LCD_DATA=data&0xff;	
#endif
	
#ifdef TFTLCD_NT35510
	TFTLCD->LCD_DATA=data;
#endif

#ifdef TFTLCD_ILI9486
	TFTLCD->LCD_DATA=data;
#endif

#ifdef TFTLCD_ILI9481
	TFTLCD->LCD_DATA=data;
#endif

}

void LCD_WriteCmdData(uint16_t cmd,uint16_t data)
{
	LCD_WriteCmd(cmd);
	LCD_WriteData(data);
}

void LCD_WriteData_Color(uint16_t color)
{
#ifdef TFTLCD_HX8357D
	TFTLCD->LCD_DATA=color;
#endif

#ifdef TFTLCD_HX8352C
	TFTLCD->LCD_DATA=color&0xff00;
	TFTLCD->LCD_DATA=color<<8;
#endif	

#ifdef TFTLCD_ILI9488
	//TFTLCD->LCD_DATA=color;
	TFTLCD->LCD_DATA=color>>8;//后续加入
	TFTLCD->LCD_DATA=color&0xff;//后续加入
	#endif
	
#ifdef TFTLCD_ILI9327
	TFTLCD->LCD_DATA=color>>8;
	TFTLCD->LCD_DATA=color&0xff;
#endif

#ifdef TFTLCD_ST7793
	TFTLCD->LCD_DATA=color>>8;
	TFTLCD->LCD_DATA=color&0xff;
#endif
	
#ifdef TFTLCD_NT35510
	TFTLCD->LCD_DATA=color;	
#endif

#ifdef TFTLCD_ILI9486
	TFTLCD->LCD_DATA=color;
#endif

#ifdef TFTLCD_ILI9481
	TFTLCD->LCD_DATA=color;
#endif
}

//uint16_t LCD_ReadData_Color()	//获取当前点的颜色
//{
//#ifdef TFTLCD_ILI9486
//	return TFTLCD->LCD_DATA;
//#endif
//}


//读数据
//返回值:读到的值
uint16_t LCD_ReadData(void)
{
#ifdef TFTLCD_HX8357D
	return TFTLCD->LCD_DATA;
#endif
	
#ifdef TFTLCD_HX8352C
	return ((TFTLCD->LCD_DATA<<8)|(TFTLCD->LCD_DATA));
#endif

#ifdef TFTLCD_ILI9488
	return TFTLCD->LCD_DATA;
#endif

#ifdef TFTLCD_ILI9327		
	uint16_t ram;
	ram=TFTLCD->LCD_DATA;
	ram=ram<<8;
	ram|=TFTLCD->LCD_DATA;	
	return ram;		
#endif

#ifdef TFTLCD_ST7793		
	uint16_t ram;
	ram=TFTLCD->LCD_DATA;
	ram=ram<<8;
	ram|=TFTLCD->LCD_DATA;	
	return ram;		
#endif
	
#ifdef TFTLCD_NT35510		
	return TFTLCD->LCD_DATA;
#endif

#ifdef TFTLCD_ILI9486
	return TFTLCD->LCD_DATA;
#endif

#ifdef TFTLCD_ILI9481
	return TFTLCD->LCD_DATA;
#endif

}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)  //默认竖屏方向
	{		
#ifdef TFTLCD_HX8357D		
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x4c);
		LcdPara.height=480;
		LcdPara.width=320;
#endif

#ifdef TFTLCD_HX8352C
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x03);  //03:U-D,L-R; 06:D-U,R-L;
		LcdPara.height=400;
		LcdPara.width=240;
#endif

#ifdef TFTLCD_ILI9488
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x48);
		LcdPara.height=480;
		LcdPara.width=320;

#endif

#ifdef TFTLCD_ILI9327
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x48);  
		LcdPara.height=400;
		LcdPara.width=240;
#endif

#ifdef TFTLCD_ST7793
		LCD_WriteCmd(0x0001);   
		LCD_WriteData(0x0100);
		LCD_WriteCmd(0x0003);
		LCD_WriteData(0x1030);
		LcdPara.height=400;
		LcdPara.width=240;
#endif
		
#ifdef TFTLCD_NT35510	
		LCD_WriteCmd(0x3600);   
		LCD_WriteData(0x0000);
		LcdPara.height=800;
		LcdPara.width=480;
#endif

#ifdef TFTLCD_ILI9486
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x00);  
		LcdPara.height=480;
		LcdPara.width=320;
#endif

#ifdef TFTLCD_ILI9481
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x00);
		LcdPara.height=480;
		LcdPara.width=320;
#endif

		LcdPara.dir=0;
	}
	else
	{	
#ifdef TFTLCD_HX8357D
		LCD_WriteCmd(0x36);   
		LCD_WriteData(0x2c);
		LcdPara.height=320;
		LcdPara.width=480;
#endif
		
#ifdef TFTLCD_HX8352C
		LCD_WriteCmd(0x16);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x60);  //60：横屏  
		LcdPara.height=240;
		LcdPara.width=400;
#endif

#ifdef TFTLCD_ILI9488
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x38);
		LcdPara.height=320;
		LcdPara.width=480;
#endif

#ifdef TFTLCD_ILI9327
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x28);  
		LcdPara.height=240;
		LcdPara.width=400;
#endif

#ifdef TFTLCD_ST7793
		LCD_WriteCmd(0x0001);   
		LCD_WriteData(0x0000);
		LCD_WriteCmd(0x0003);
		LCD_WriteData(0x1038);
		LcdPara.height=240;
		LcdPara.width=400;
#endif

#ifdef TFTLCD_NT35510	
		LCD_WriteCmd(0x3600);   
		LCD_WriteData(0x0060);
		LcdPara.height=480;
		LcdPara.width=800;
#endif

#ifdef TFTLCD_ILI9486
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x60);  
		LcdPara.height=320;
		LcdPara.width=480;
#endif

#ifdef TFTLCD_ILI9481
		LCD_WriteCmd(0x36);   //设置彩屏显示方向的寄存器
		LCD_WriteData(0x60);
		LcdPara.height=320;
		LcdPara.width=480;
#endif

		LcdPara.dir=1;
	}	

//使用全局变量
	LCD_W=LcdPara.width;		//LCD 宽度
	LCD_H=LcdPara.height;		//LCD 高度
}

#ifdef TFTLCD_HX8357D
void LCDInit_HX8357D()
{
	LCD_WriteCmd(0xE9);
	LCD_WriteData(0x20);

	LCD_WriteCmd(0x11); //Exit Sleep
	for(i=500; i>0; i--);

	LCD_WriteCmd(0x3A);
	LCD_WriteData(0x55);  //16Bit colors

	LCD_WriteCmd(0xD1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x65); //调试此值改善水纹
	LCD_WriteData(0x1F);

	LCD_WriteCmd(0xD0);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);
	LCD_WriteData(0x80);

	LCD_WriteCmd(0x36); 	 //Set_address_mode
	LCD_WriteData(0x4c);   	//4c

	LCD_WriteCmd(0xC1);
	LCD_WriteData(0x10);
	LCD_WriteData(0x10);
	LCD_WriteData(0x02);
	LCD_WriteData(0x02);

	LCD_WriteCmd(0xC0); //Set Default Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x35);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x02);

	LCD_WriteCmd(0xC4);
	LCD_WriteData(0x03);

	LCD_WriteCmd(0xC5); //Set frame rate
	LCD_WriteData(0x01);

	LCD_WriteCmd(0xD2); //power setting
	LCD_WriteData(0x01);
	LCD_WriteData(0x22);

	LCD_WriteCmd(0xE7);
	LCD_WriteData(0x38);

	LCD_WriteCmd(0xF3);
    LCD_WriteData(0x08);
	LCD_WriteData(0x12);
	LCD_WriteData(0x12);
	LCD_WriteData(0x08);

	LCD_WriteCmd(0xC8); //Set Gamma
	LCD_WriteData(0x01);
	LCD_WriteData(0x52);
	LCD_WriteData(0x37);
	LCD_WriteData(0x10);
	LCD_WriteData(0x0d);
	LCD_WriteData(0x01);
	LCD_WriteData(0x04);
	LCD_WriteData(0x51);
	LCD_WriteData(0x77);
	LCD_WriteData(0x01);
	LCD_WriteData(0x01);
	LCD_WriteData(0x0d);
	LCD_WriteData(0x08);
	LCD_WriteData(0x80);
	LCD_WriteData(0x00);

    LCD_WriteCmd(0x29);
}
#endif


#ifdef TFTLCD_HX8352C
void LCDInit_HX8352C()
{
	//************* Start Initial Sequence **********//	
	//##################################################################
	//Power Voltage Setting
	LCD_WriteCmdData(0x1A,0x02); //BT  0x02
	LCD_WriteCmdData(0x1B,0x88); //VRH  0x88
	LCD_WriteCmdData(0x1C,0x06); //AP[0:2]  //自己添加
	
	//****VCOM offset**///
	LCD_WriteCmdData(0x23,0x00); //SEL_VCM   00
	LCD_WriteCmdData(0x24,0x5f); //VCM    5f
	LCD_WriteCmdData(0x25,0x15); //VDV   0x15
	LCD_WriteCmdData(0x2D,0x03); //NOW[2:0]=011
	//Power on Setting
	LCD_WriteCmdData(0x18,0x04); //Frame rate 72Hz
	LCD_WriteCmdData(0x19,0x01); //OSC_EN='1', start Osc
	LCD_WriteCmdData(0x01,0x00); //DP_STB='0', out deep sleep
	LCD_WriteCmdData(0x1F,0x88); //STB=0
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0x80); //DK=0
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0x90); //PON=1
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0xD0); //VCOMG=1
	HAL_Delay(5);
	//262k/65k color selection
	LCD_WriteCmdData(0x17,0x05); //default 0x06 262k color // 0x05 65k color
	//SET PANEL
	LCD_WriteCmdData(0x36,0x13); //REV_P, SM_P, GS_P, BGR_P, SS_P   0x03
	LCD_WriteCmdData(0x29,0x31); //400 lines
	LCD_WriteCmdData(0x71,0x1A); //RTN
	//Gamma 2.2 Setting
	LCD_WriteCmdData(0x40,0x00);
	LCD_WriteCmdData(0x41,0x45);
	LCD_WriteCmdData(0x42,0x45);
	LCD_WriteCmdData(0x43,0x04);
	LCD_WriteCmdData(0x44,0x00);
	LCD_WriteCmdData(0x45,0x08);
	LCD_WriteCmdData(0x46,0x23);
	LCD_WriteCmdData(0x47,0x23);
	LCD_WriteCmdData(0x48,0x77);
	LCD_WriteCmdData(0x49,0x40);
	LCD_WriteCmdData(0x4A,0x04);
	LCD_WriteCmdData(0x4B,0x00);
	LCD_WriteCmdData(0x4C,0x88);
	LCD_WriteCmdData(0x4D,0x88);
	LCD_WriteCmdData(0x4E,0x88);
	//Set GRAM Area
	LCD_WriteCmdData(0x02,0x00);
	LCD_WriteCmdData(0x03,0x00); //Column Start
	LCD_WriteCmdData(0x04,0x00);
	LCD_WriteCmdData(0x05,0xEF); //Column End
	LCD_WriteCmdData(0x06,0x00);
	LCD_WriteCmdData(0x07,0x00); //Row Start
	LCD_WriteCmdData(0x08,0x01);
	LCD_WriteCmdData(0x09,0x8F); //Row End  //AF
	//Display ON Setting
	LCD_WriteCmdData(0x28,0x38); //GON=1, DTE=1, D=10
	HAL_Delay(40);
	LCD_WriteCmdData(0x28,0x3F); //GON=1, DTE=1, D=11  0x3C
	LCD_WriteCmd(0x22);
	 //Start GRAM write
	//##################################################################
	// Power Off Setting
	LCD_WriteCmdData(0x28,0x38); //GON=’1’ DTE=’1’ D[1:0]=’10’
	HAL_Delay(40);
	LCD_WriteCmdData(0x1F,0x89); // GAS=1, VOMG=0, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=1
	HAL_Delay(40);
	LCD_WriteCmdData(0x28,0x20); //GON=’1’ DTE=’0’ D[1:0]=’00’
	HAL_Delay(40);
	LCD_WriteCmdData(0x19,0x00); //OSC_EN=’0’
	HAL_Delay(40);
	//M51_WR_REG(U05_LCD_POWER_ON, 0x00); //VCI & IOVCC OFF
	//##################################################################
	// ENTER IDLE mode Setting
	LCD_WriteCmdData(0x18,0x07); //Frame Rate 60Hz
	LCD_WriteCmdData(0x01,0x04); //IDLE='1' , enter IDLE mode
	// EXIT IDLE mode Setting
	LCD_WriteCmdData(0x18,0x04); //Frame rate 72Hz
	LCD_WriteCmdData(0x01,0x00); //IDLE='0', EXIT IDLE mode
	//##################################################################
	// ENTER Partial mode Setting 32line
	LCD_WriteCmdData(0x18,0x04); //Frame rate 72Hz
	LCD_WriteCmdData(0x01,0x01); //PTL='1', Enter Partial mode
	LCD_WriteCmdData(0x0A,0x00); //PSL[15:8]=0x00
	LCD_WriteCmdData(0x0B,0x20); //PSL[7:0]=0x20
	LCD_WriteCmdData(0x0C,0x00); //PEL[15:8]=0x00
	LCD_WriteCmdData(0x0D,0x3F); //PEL[7:0]=0x3F
	// EXIT Partial mode Setting
	LCD_WriteCmdData(0x18,0x04); //Frame rate 72Hz
	LCD_WriteCmdData(0x01,0x00); //PTL='0', EXIT Partial mode
	//##################################################################
	// ENTER Partial + IDLE mode Setting 32line
	LCD_WriteCmdData(0x18,0x07); //Frame Rate 60Hz
	LCD_WriteCmdData(0x01,0x05); // PTL='1' IDLE=’1’, Enter Partial + IDLE mode
	LCD_WriteCmdData(0x0A,0x00); //PSL[15:8]=0x00
	LCD_WriteCmdData(0x0B,0x20); //PSL[7:0]=0x20
	LCD_WriteCmdData(0x0C,0x00); //PEL[15:8]=0x00
	LCD_WriteCmdData(0x0D,0x3F); //PEL[7:0]=0x3F
	// EXIT Partial + IDLE mode Setting
	LCD_WriteCmdData(0x18,0x04); //Frame rate 72Hz
	LCD_WriteCmdData(0x01,0x00); // PTL='0' IDLE=’0’, EXIT Partial + IDLE mode
	//##################################################################
	// Enter Sleep mode Setting
	LCD_WriteCmdData(0x28,0x38); //GON=’1’ DTE=’1’ D[1:0]=’10’
	HAL_Delay(40);
	LCD_WriteCmdData(0x1F,0x89); // GAS=1, VOMG=0, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=1
	HAL_Delay(40);
	LCD_WriteCmdData(0x28,0x20); //GON=’1’ DTE=’0’ D[1:0]=’00’
	HAL_Delay(40);
	LCD_WriteCmdData(0x19,0x00); //OSC_EN=’0’
	HAL_Delay(5);
	// Exit Sleep mode Setting
	LCD_WriteCmdData(0x19,0x01); //OSC_EN='1', start Osc
	LCD_WriteCmdData(0x1F,0x88); //STB=0
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0x80); //DK=0
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0x90); //PON=1
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0xD0); //VCOMG=1
	HAL_Delay(5);
	LCD_WriteCmdData(0x28,0x38); //GON=1, DTE=1, D=10
	HAL_Delay(40);
	LCD_WriteCmdData(0x28,0x3F); //GON=1, DTE=1, D=11
	//##################################################################
	// Enter Deep Sleep mode Setting
	LCD_WriteCmdData(0x28,0x38); //GON=’1’ DTE=’1’ D[1:0]=’10’
	HAL_Delay(40);
	LCD_WriteCmdData(0x1F,0x89); // GAS=1, VOMG=0, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=1
	HAL_Delay(40);
	LCD_WriteCmdData(0x28,0x20); //GON=’1’ DTE=’0’ D[1:0]=’00’
	HAL_Delay(40);
	LCD_WriteCmdData(0x19,0x00); //OSC_EN=’0’
	HAL_Delay(5);
	LCD_WriteCmdData(0x01,0x80); //DP_STB=’1’
	// Exit Deep Sleep mode Setting
	LCD_WriteCmdData(0x01,0x00); //DP_STB='0', out deep sleep
	HAL_Delay(10);
	LCD_WriteCmdData(0x19,0x01); //OSC_EN='1', start Osc
	LCD_WriteCmdData(0x1F,0x88); //STB=0
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0x80); //DK=0
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0x90); //PON=1
	HAL_Delay(5);
	LCD_WriteCmdData(0x1F,0xD0); //VCOMG=1
	HAL_Delay(5);
	LCD_WriteCmdData(0x28,0x38); //GON=1, DTE=1, D=10
	HAL_Delay(40);
	LCD_WriteCmdData(0x28,0x3C); //GON=1, DTE=1, D=11

	LCD_WriteCmd(0x22);
}

#endif

#ifdef TFTLCD_ILI9488
void	LCDInit_ILI9488()
{
//ILI9488+CTC3.5
//************* Start Initial Sequence **********//
	LCD_WriteCmd(0xE0);
	LCD_WriteData(0x00);
	LCD_WriteData(0x07);
	LCD_WriteData(0x0f);
	LCD_WriteData(0x0D);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x3c);
	LCD_WriteData(0x78);
	LCD_WriteData(0x4A);
	LCD_WriteData(0x07);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x09);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x1e);
	LCD_WriteData(0x0f);

	LCD_WriteCmd(0xE1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x22);
	LCD_WriteData(0x24);
	LCD_WriteData(0x06);
	LCD_WriteData(0x12);
	LCD_WriteData(0x07);
	LCD_WriteData(0x36);
	LCD_WriteData(0x47);
	LCD_WriteData(0x47);
	LCD_WriteData(0x06);
	LCD_WriteData(0x0a);
	LCD_WriteData(0x07);
	LCD_WriteData(0x30);
	LCD_WriteData(0x37);
	LCD_WriteData(0x0f);

	LCD_WriteCmd(0xC0);
	LCD_WriteData(0x10);
	LCD_WriteData(0x10);

	LCD_WriteCmd(0xC1);
	LCD_WriteData(0x41);

	LCD_WriteCmd(0xC5);
	LCD_WriteData(0x00);
	LCD_WriteData(0x22);
	LCD_WriteData(0x80);

	LCD_WriteCmd(0x36);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x3A); //Interface Mode Control
	LCD_WriteData(0x55);

	LCD_WriteCmd(0XB0);  //Interface Mode Control
	LCD_WriteData(0x00);
	LCD_WriteCmd(0xB1);   //Frame rate 70HZ
	LCD_WriteData(0xB0);
	LCD_WriteData(0x11);
	LCD_WriteCmd(0xB4);
	LCD_WriteData(0x02);
	LCD_WriteCmd(0xB6); //RGB/MCU Interface Control
	LCD_WriteData(0x02);
	LCD_WriteData(0x22);//LCD_WriteData(0x02);
						//上行修改成0x22
	LCD_WriteCmd(0xB7);
	LCD_WriteData(0xC6);

	//LCD_WriteCmd(0XBE);
	//LCD_WriteData(0x00);
	//LCD_WriteData(0x04);

	LCD_WriteCmd(0xE9);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0XF7);
	LCD_WriteData(0xA9);
	LCD_WriteData(0x51);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x82);

	LCD_WriteCmd(0x11);
	HAL_Delay(120);
	LCD_WriteCmd(0x29);
}
#endif

#ifdef TFTLCD_ILI9327
void	LCDInit_ILI9327()
{
	//************* Start Initial Sequence **********//

	LCD_WriteCmd(0xE9);
	LCD_WriteData(0x20);

	LCD_WriteCmd(0x11); //Exit Sleep
	HAL_Delay(100);

	LCD_WriteCmd(0x3A);
	LCD_WriteData(0x55);

	LCD_WriteCmd(0xD1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x65); //调试此值改善水纹
	LCD_WriteData(0x1F);

	LCD_WriteCmd(0xD0);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);
	LCD_WriteData(0x80);

	LCD_WriteCmd(0x36);
	LCD_WriteData(0x48);

	LCD_WriteCmd(0xC1);
	LCD_WriteData(0x10);
	LCD_WriteData(0x10);
	LCD_WriteData(0x02);
	LCD_WriteData(0x02);

	LCD_WriteCmd(0xC0); //Set Default Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x35);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x02);

	LCD_WriteCmd(0xC5); //Set frame rate
	LCD_WriteData(0x01);

	LCD_WriteCmd(0xD2); //power setting
	LCD_WriteData(0x01);
	LCD_WriteData(0x22);

	LCD_WriteCmd(0xC8); //Set Gamma
	LCD_WriteData(0x01);
	LCD_WriteData(0x52);
	LCD_WriteData(0x37);
	LCD_WriteData(0x10);
	LCD_WriteData(0x0d);
	LCD_WriteData(0x01);
	LCD_WriteData(0x04);
	LCD_WriteData(0x51);
	LCD_WriteData(0x77);
	LCD_WriteData(0x01);
	LCD_WriteData(0x01);
	LCD_WriteData(0x0d);
	LCD_WriteData(0x08);
	LCD_WriteData(0x80);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x29); //display on
}
#endif


#ifdef TFTLCD_ST7793
void	LCDInit_ST7793()
{
	for(i=500; i>0; i--);
	LCD_WriteCmd(0x0001);
	LCD_WriteData(0x0100);
	LCD_WriteCmd(0x0003);
	LCD_WriteData(0x1030); //16bit 565

	LCD_WriteCmd(0x0008);
	LCD_WriteData(0x0808);
	LCD_WriteCmd(0x0090);
	LCD_WriteData(0x8000);
	LCD_WriteCmd(0x0400);
	LCD_WriteData(0x6200);
	LCD_WriteCmd(0x0401);
	LCD_WriteData(0x0001);

	LCD_WriteCmd(0x00ff);
	LCD_WriteData(0x0001);
	LCD_WriteCmd(0x0102);
	LCD_WriteData(0x01b0);
	LCD_WriteCmd(0x0710);
	LCD_WriteData(0x0016);
	LCD_WriteCmd(0x0712);
	LCD_WriteData(0x000f);
	LCD_WriteCmd(0x0752);
	LCD_WriteData(0x002f);
	LCD_WriteCmd(0x0724);
	LCD_WriteData(0x001a);
	LCD_WriteCmd(0x0754);
	LCD_WriteData(0x0018);
	for(i=500; i>0; i--);

    /* Gamma */
	LCD_WriteCmd(0x0380);
	LCD_WriteData(0x0000);
	LCD_WriteCmd(0x0381);
	LCD_WriteData(0x5F10);//5f10
	LCD_WriteCmd(0x0382);
	LCD_WriteData(0x0B02);//0b02
	LCD_WriteCmd(0x0383);
	LCD_WriteData(0x0614);
	LCD_WriteCmd(0x0384);
	LCD_WriteData(0x0111);
	LCD_WriteCmd(0x0385);
	LCD_WriteData(0x0000);
	LCD_WriteCmd(0x0386);
	LCD_WriteData(0xA90B);//a90b
	LCD_WriteCmd(0x0387);
	LCD_WriteData(0x0606);//0606
	LCD_WriteCmd(0x0388);
	LCD_WriteData(0x0612);
	LCD_WriteCmd(0x0389);
	LCD_WriteData(0x0111);


    /* Gamma Setting */
    /* Setting */
	LCD_WriteCmd(0x0702);
	LCD_WriteData(0x003b);//003b
	LCD_WriteCmd(0x00ff);
	LCD_WriteData(0x0000);


    /* Vcom Setting */
	LCD_WriteCmd(0x0007);
	LCD_WriteData(0x0100);
	for(i=500; i>0; i--);
	LCD_WriteCmd(0x0200);
	LCD_WriteData(0x0000);
	LCD_WriteCmd(0x0201);
	LCD_WriteData(0x0000);
}
#endif


#ifdef TFTLCD_NT35510
void	LCDInit_NT35510()
{
	//************* Start Initial Sequence **********//
	//**********LV2 Page 1 enable***********//
	LCD_WriteCmd(0xF000);	LCD_WriteData(0x0055);
	LCD_WriteCmd(0xF001);	LCD_WriteData(0x00AA);
	LCD_WriteCmd(0xF002);	LCD_WriteData(0x0052);
	LCD_WriteCmd(0xF003);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xF004);	LCD_WriteData(0x0001);

	//*************AVDD Set AVDD 5.2V*************//
	LCD_WriteCmd(0xB000);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB001);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB002);	LCD_WriteData(0x0000);

	//************AVDD ratio****************//
	LCD_WriteCmd(0xB600);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB601);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB602);	LCD_WriteData(0x0034);
	 
	//************AVEE  -5.2V****************//
	LCD_WriteCmd(0xB100);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB101);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB102);	LCD_WriteData(0x0000);

	//***********AVEE ratio*************//
	LCD_WriteCmd(0xB700);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB701);	LCD_WriteData(0x0034);
	LCD_WriteCmd(0xB702);	LCD_WriteData(0x0034);

	//***********VCL  -2.5V*************//
	LCD_WriteCmd(0xB200);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB201);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xB202);	LCD_WriteData(0x0000);

	//**************VCL ratio*****************//
	LCD_WriteCmd(0xB800);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xB801);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xB802);	LCD_WriteData(0x0005);


	//*************VGH 15V  (Free pump)*********//
	LCD_WriteCmd(0xBF00);	LCD_WriteData(0x0001);
	LCD_WriteCmd(0xB300);	LCD_WriteData(0x000F);
	LCD_WriteCmd(0xB301);	LCD_WriteData(0x000F);
	LCD_WriteCmd(0xB302);	LCD_WriteData(0x000F);

	//*************VGH ratio*****************//
	LCD_WriteCmd(0xB900);	LCD_WriteData(0x0024);
	LCD_WriteCmd(0xB901);	LCD_WriteData(0x0024);
	LCD_WriteCmd(0xB902);	LCD_WriteData(0x0024);

	//***************VGL_REG -10V**************//
	LCD_WriteCmd(0xB500);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xB501);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xB502);	LCD_WriteData(0x0008);

//	LCD_WriteCmd(0xC2,0x00);	LCD_WriteData(0x00,0x03);
		 
	//*****************VGLX ratio***************//
	LCD_WriteCmd(0xBA00);	LCD_WriteData(0x0014);
	LCD_WriteCmd(0xBA01);	LCD_WriteData(0x0014);
	LCD_WriteCmd(0xBA02);	LCD_WriteData(0x0014);

	//*************VGMP/VGSP 4.5V/0V*************//
	LCD_WriteCmd(0xBC00);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xBC01);	LCD_WriteData(0x00BC);
	LCD_WriteCmd(0xBC02);	LCD_WriteData(0x0000);

	//************VGMN/VGSN -4.5V/0V****************//
	LCD_WriteCmd(0xBD00);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xBD01);	LCD_WriteData(0x00BC);
	LCD_WriteCmd(0xBD02);	LCD_WriteData(0x0000);

	//************VCOM  -1.25V****************//
	LCD_WriteCmd(0xBE00);	LCD_WriteData(0x0000);
	LCD_WriteCmd(0xBE01);	LCD_WriteData(0x002D);

	//************Gamma Setting******************//

	LCD_WriteCmd(0xD100); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD101); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD102); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD103); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD104); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD105); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD106); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD107); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD108); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD109); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD10A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD10B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD10C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD10D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD10E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD10F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD110); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD111); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD112); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD113); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD114); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD115); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD116); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD117); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD118); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD119); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD11A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD11B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD11C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD11D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD11E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD11F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD120); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD121); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD122); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD123); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD124); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD125); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD126); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD127); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD128); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD129); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD12A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD12B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD12C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD12D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD12E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD12F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD130); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD131); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD132); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD133); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD200); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD201); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD202); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD203); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD204); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD205); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD206); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD207); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD208); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD209); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD20A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD20B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD20C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD20D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD20E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD20F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD210); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD211); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD212); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD213); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD214); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD215); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD216); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD217); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD218); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD219); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD21A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD21B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD21C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD21D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD21E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD21F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD220); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD221); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD222); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD223); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD224); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD225); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD226); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD227); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD228); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD229); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD22A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD22B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD22C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD22D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD22E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD22F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD230); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD231); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD232); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD233); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD300); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD301); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD302); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD303); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD304); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD305); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD306); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD307); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD308); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD309); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD30A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD30B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD30C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD30D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD30E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD30F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD310); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD311); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD312); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD313); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD314); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD315); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD316); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD317); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD318); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD319); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD31A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD31B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD31C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD31D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD31E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD31F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD320); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD321); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD322); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD323); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD324); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD325); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD326); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD327); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD328); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD329); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD32A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD32B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD32C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD32D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD32E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD32F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD330); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD331); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD332); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD333); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD400); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD401); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD402); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD403); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD404); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD405); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD406); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD407); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD408); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD409); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD40A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD40B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD40C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD40D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD40E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD40F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD410); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD411); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD412); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD413); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD414); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD415); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD416); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD417); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD418); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD419); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD41A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD41B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD41C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD41D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD41E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD41F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD420); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD421); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD422); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD423); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD424); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD425); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD426); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD427); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD428); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD429); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD42A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD42B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD42C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD42D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD42E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD42F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD430); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD431); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD432); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD433); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD500); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD501); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD502); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD503); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD504); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD505); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD506); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD507); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD508); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD509); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD50A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD50B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD50C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD50D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD50E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD50F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD510); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD511); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD512); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD513); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD514); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD515); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD516); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD517); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD518); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD519); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD51A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD51B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD51C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD51D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD51E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD51F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD520); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD521); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD522); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD523); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD524); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD525); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD526); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD527); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD528); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD529); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD52A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD52B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD52C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD52D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD52E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD52F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD530); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD531); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD532); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD533); LCD_WriteData(0x00C1);

	LCD_WriteCmd(0xD600); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD601); LCD_WriteData(0x0037);
	LCD_WriteCmd(0xD602); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD603); LCD_WriteData(0x0049);
	LCD_WriteCmd(0xD604); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD605); LCD_WriteData(0x006B);
	LCD_WriteCmd(0xD606); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD607); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD608); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD609); LCD_WriteData(0x009D);
	LCD_WriteCmd(0xD60A); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD60B); LCD_WriteData(0x00C3);
	LCD_WriteCmd(0xD60C); LCD_WriteData(0x0000);
	LCD_WriteCmd(0xD60D); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD60E); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD60F); LCD_WriteData(0x000D);
	LCD_WriteCmd(0xD610); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD611); LCD_WriteData(0x002F);
	LCD_WriteCmd(0xD612); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD613); LCD_WriteData(0x0061);
	LCD_WriteCmd(0xD614); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD615); LCD_WriteData(0x0086);
	LCD_WriteCmd(0xD616); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD617); LCD_WriteData(0x00BD);
	LCD_WriteCmd(0xD618); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD619); LCD_WriteData(0x00E7);
	LCD_WriteCmd(0xD61A); LCD_WriteData(0x0001);
	LCD_WriteCmd(0xD61B); LCD_WriteData(0x00E8);
	LCD_WriteCmd(0xD61C); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD61D); LCD_WriteData(0x000C);
	LCD_WriteCmd(0xD61E); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD61F); LCD_WriteData(0x0030);
	LCD_WriteCmd(0xD620); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD621); LCD_WriteData(0x0045);
	LCD_WriteCmd(0xD622); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD623); LCD_WriteData(0x0063);
	LCD_WriteCmd(0xD624); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD625); LCD_WriteData(0x007E);
	LCD_WriteCmd(0xD626); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD627); LCD_WriteData(0x00B1);
	LCD_WriteCmd(0xD628); LCD_WriteData(0x0002);
	LCD_WriteCmd(0xD629); LCD_WriteData(0x00E0);
	LCD_WriteCmd(0xD62A); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD62B); LCD_WriteData(0x0026);
	LCD_WriteCmd(0xD62C); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD62D); LCD_WriteData(0x0060);
	LCD_WriteCmd(0xD62E); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD62F); LCD_WriteData(0x00B4);
	LCD_WriteCmd(0xD630); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD631); LCD_WriteData(0x00B9);
	LCD_WriteCmd(0xD632); LCD_WriteData(0x0003);
	LCD_WriteCmd(0xD633); LCD_WriteData(0x00C1);


	//**************LV2 Page 0 enable*************//
	LCD_WriteCmd(0xF000);	LCD_WriteData(0x0055);
	LCD_WriteCmd(0xF001);	LCD_WriteData(0x00AA);
	LCD_WriteCmd(0xF002);	LCD_WriteData(0x0052);
	LCD_WriteCmd(0xF003);	LCD_WriteData(0x0008);
	LCD_WriteCmd(0xF004);	LCD_WriteData(0x0000);

	//*************480x800*********************//
	LCD_WriteCmd(0xB500);	LCD_WriteData(0x0050);

	//***************Display control**************//
	LCD_WriteCmd(0xB100);	LCD_WriteData(0x00CC);
	//LCD_WriteCmd(0xB101);	LCD_WriteData(0x0000);

	//***************Source hold time*************//
	LCD_WriteCmd(0xB600);	LCD_WriteData(0x0002);

	//**************Gate EQ control***************//
	//LCD_WriteCmd(0xB700);	LCD_WriteData(0x0070);
	//LCD_WriteCmd(0xB701);	LCD_WriteData(0x0070);

	//*************Source EQ control (Mode 2)******//
	LCD_WriteCmd(0xB800);	LCD_WriteData(0x0001);
	LCD_WriteCmd(0xB801);	LCD_WriteData(0x0003);
	LCD_WriteCmd(0xB802);	LCD_WriteData(0x0003);
	LCD_WriteCmd(0xB803);	LCD_WriteData(0x0003);
					
	//************Inversion mode  (2-dot)***********//
	LCD_WriteCmd(0xBC00);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xBC01);	LCD_WriteData(0x0005);
	LCD_WriteCmd(0xBC02);	LCD_WriteData(0x0005);
					 
	//***************Frame rate***************//      
	//LCD_WriteCmd(0xBD00);	LCD_WriteData(0x0001);
	//LCD_WriteCmd(0xBD01);	LCD_WriteData(0x0084);
	LCD_WriteCmd(0xBD02);	LCD_WriteData(0x0007);  //0X1C
	LCD_WriteCmd(0xBD03);	LCD_WriteData(0x0031);
	LCD_WriteCmd(0xBE02);	LCD_WriteData(0x0007);  
	LCD_WriteCmd(0xBE03);	LCD_WriteData(0x0031);
	LCD_WriteCmd(0xBF02);	LCD_WriteData(0x0007);
	LCD_WriteCmd(0xBE03);	LCD_WriteData(0x0031);
	
	
	LCD_WriteCmd(0xFF00);	LCD_WriteData(0x00AA);
	LCD_WriteCmd(0xFF01);	LCD_WriteData(0x0055);    
	LCD_WriteCmd(0xFF02);	LCD_WriteData(0x0025);
	LCD_WriteCmd(0xFF03);	LCD_WriteData(0x0001);
	
	LCD_WriteCmd(0xF306);	LCD_WriteData(0x0010);
	LCD_WriteCmd(0xF408);	LCD_WriteData(0x0000);
	

	//********Timing control 4H w/ 4-Delayms *******//
	/*
	LCD_WriteCmd(0xC900);	LCD_WriteData(0x00D0);
	LCD_WriteCmd(0xC901);	LCD_WriteData(0x0002); 
	LCD_WriteCmd(0xC902);	LCD_WriteData(0x0050);
	LCD_WriteCmd(0xC903);	LCD_WriteData(0x0050);
	LCD_WriteCmd(0xC904);	LCD_WriteData(0x0050);
	*/
	LCD_WriteCmd(0x3600);	LCD_WriteData(0x00C0);//调整扫描方式和RGB  BGR
	LCD_WriteCmd(0x3500);	LCD_WriteData(0x0000);
	
	LCD_WriteCmd(0x3A00);	LCD_WriteData(0x0005);	//70
	 
	LCD_WriteCmd(0x1100);
	HAL_Delay(120);
	LCD_WriteCmd(0x2900);
	HAL_Delay(50);
}
#endif


#ifdef TFTLCD_ILI9486
void	LCDInit_ILI9486()
{
	LCD_WriteCmd(0xB0);
	LCD_WriteData(0x00);
	LCD_WriteCmd(0x11);
	HAL_Delay(20);

	LCD_WriteCmd(0xB3);
	LCD_WriteData(0x02);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xC0);
	LCD_WriteData(0x10);//13
	LCD_WriteData(0x3B);//480
	LCD_WriteData(0x00);
	LCD_WriteData(0x02);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);//NW
	LCD_WriteData(0x43);

	LCD_WriteCmd(0xC1);
	LCD_WriteData(0x10);//LCD_WriteData(0x08);
	LCD_WriteData(0x10);//LCD_WriteData(0x16);//CLOCK
	LCD_WriteData(0x08);
	LCD_WriteData(0x08);

	LCD_WriteCmd(0xC4);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x03);
	LCD_WriteData(0x03);

	LCD_WriteCmd(0xC6);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xC8);//GAMMA
	LCD_WriteData(0x03);
	LCD_WriteData(0x03);
	LCD_WriteData(0x13);
	LCD_WriteData(0x5C);
	LCD_WriteData(0x03);
	LCD_WriteData(0x07);
	LCD_WriteData(0x14);
	LCD_WriteData(0x08);
	LCD_WriteData(0x00);
	LCD_WriteData(0x21);
	LCD_WriteData(0x08);
	LCD_WriteData(0x14);
	LCD_WriteData(0x07);
	LCD_WriteData(0x53);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x13);
	LCD_WriteData(0x03);
	LCD_WriteData(0x03);
	LCD_WriteData(0x21);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x35);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x36);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x3A);
	LCD_WriteData(0x55);

	LCD_WriteCmd(0x44);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);

	LCD_WriteCmd(0xB6);
	LCD_WriteData(0x00);
	LCD_WriteData(0x22);//0 GS SS SM ISC[3:0];其中GS SS控制显示方向，同时修改R36
	LCD_WriteData(0x3B);

	LCD_WriteCmd(0xD0);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);//VCI1
	LCD_WriteData(0x1D);//VRH

	LCD_WriteCmd(0xD1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x03);//VCM
	LCD_WriteData(0x00);//VDV

	LCD_WriteCmd(0xD2);
	LCD_WriteData(0x03);
	LCD_WriteData(0x14);
	LCD_WriteData(0x04);

	LCD_WriteCmd(0x29);
	HAL_Delay(20);

	LCD_WriteCmd(0xB4);
	LCD_WriteData(0x00);
	HAL_Delay(20);
	LCD_WriteCmd(0x2C);
}
#endif


#ifdef TFTLCD_ILI9481
void	LCDInit_ILI9481()
{
	LCD_WriteCmd(0xFF);
	LCD_WriteCmd(0xFF);
	HAL_Delay(5);

	LCD_WriteCmd(0xFF);
	LCD_WriteCmd(0xFF);
	LCD_WriteCmd(0xFF);
	LCD_WriteCmd(0xFF);
	HAL_Delay(10);

	LCD_WriteCmd(0xB0);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xB3);
	LCD_WriteData(0x02);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xC0);
	LCD_WriteData(0x13);
	LCD_WriteData(0x3B);//480
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);//NW
	LCD_WriteData(0x43);

	LCD_WriteCmd(0xC1);
	LCD_WriteData(0x08);
	LCD_WriteData(0x1B);//CLOCK
	LCD_WriteData(0x08);
	LCD_WriteData(0x08);

	LCD_WriteCmd(0xC4);
	LCD_WriteData(0x11);
	LCD_WriteData(0x01);
	LCD_WriteData(0x73);
	LCD_WriteData(0x01);

	LCD_WriteCmd(0xC6);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xC8);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x05);
	LCD_WriteData(0x14);
	LCD_WriteData(0x5C);
	LCD_WriteData(0x03);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);
	LCD_WriteData(0x10);
	LCD_WriteData(0x00);
	LCD_WriteData(0x23);

	LCD_WriteData(0x10);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);
	LCD_WriteData(0x53);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x14);
	LCD_WriteData(0x05);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x23);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x35);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0x44);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);

	LCD_WriteCmd(0xD0);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);//VCI1
	LCD_WriteData(0x1D);//VRH
	LCD_WriteData(0x03);//BT

	LCD_WriteCmd(0xD1);
	LCD_WriteData(0x03);
	LCD_WriteData(0x5B);//VCM
	LCD_WriteData(0x10);//VDV

	LCD_WriteCmd(0xD2);
	LCD_WriteData(0x03);
	LCD_WriteData(0x24);
	LCD_WriteData(0x04);

	LCD_WriteCmd(0x2A);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x3F);//320

	LCD_WriteCmd(0x2B);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0xDF);//480

	LCD_WriteCmd(0x36);
	LCD_WriteData(0x00);

	LCD_WriteCmd(0xC0);
	LCD_WriteData(0x13);

	LCD_WriteCmd(0x3A);
	LCD_WriteData(0x55);

	LCD_WriteCmd(0x11);
	HAL_Delay(150);

	LCD_WriteCmd(0x29);
	HAL_Delay(30);

	LCD_WriteCmd(0x2C);
}
#endif

// TFT LCD软件初始化
void TFTLCD_Init(void)
{
	HAL_Delay(50);

#ifdef TFTLCD_HX8357D
	LCD_WriteCmd(0Xd0);
	LcdPara.id=LCD_ReadData();	//dummy read
	LcdPara.id=LCD_ReadData();
#endif

#ifdef TFTLCD_HX8352C
	LCD_WriteCmd(0X00);
	LcdPara.id=TFTLCD->LCD_DATA>>8;		//=0x0072
#endif

#ifdef TFTLCD_ILI9488
	LCD_WriteCmd(0Xd3);
	LcdPara.id=TFTLCD->LCD_DATA;	//dummy read
	LcdPara.id=TFTLCD->LCD_DATA;  //00
	LcdPara.id=TFTLCD->LCD_DATA;  //94
	LcdPara.id<<=8;
	LcdPara.id|=TFTLCD->LCD_DATA;  //88
#endif

#ifdef TFTLCD_ILI9327
	LCD_WriteCmd(0XEF);
	LcdPara.id=TFTLCD->LCD_DATA;	//dummy read
	LcdPara.id=TFTLCD->LCD_DATA;  //02
	LcdPara.id=TFTLCD->LCD_DATA;  //04
	LcdPara.id=TFTLCD->LCD_DATA;  //93
	LcdPara.id=(LcdPara.id<<8)|TFTLCD->LCD_DATA;  //27
#endif

#ifdef TFTLCD_ST7793
	LCD_WriteCmd(0X0000);
	LcdPara.id=LCD_ReadData();
#endif

#ifdef TFTLCD_NT35510
	LCD_WriteCmd(0XDA00);
	LcdPara.id=LCD_ReadData();		//读回0X00
	LCD_WriteCmd(0XDB00);
	LcdPara.id=LCD_ReadData();		//读回0X80
	LcdPara.id<<=8;
	LCD_WriteCmd(0XDC00);
	LcdPara.id|=LCD_ReadData();		//读回0X00
#endif

#ifdef TFTLCD_ILI9486
	LCD_WriteCmd(0Xd3);
	LcdPara.id=TFTLCD->LCD_DATA;	//dummy read
	LcdPara.id=TFTLCD->LCD_DATA;  //00
	LcdPara.id=TFTLCD->LCD_DATA;  //94
	LcdPara.id<<=8;
	LcdPara.id|=TFTLCD->LCD_DATA;  //86
#endif

#ifdef TFTLCD_ILI9481
	LCD_WriteCmd(0Xd3);
	LcdPara.id=TFTLCD->LCD_DATA;
	LcdPara.id=TFTLCD->LCD_DATA;
	LcdPara.id=TFTLCD->LCD_DATA;
	LcdPara.id<<=8;
	LcdPara.id|=TFTLCD->LCD_DATA;
#endif

// 	printf(" LCD ID:%x\r\n",tftlcd_data.id); //打印LCD ID

//根据LCD  ID进行软件初始化

#ifdef TFTLCD_HX8357D
	LCDInit_HX8357D();
#endif

#ifdef TFTLCD_HX8352C
	LCDInit_HX8352C();
#endif

#ifdef TFTLCD_ILI9488
	LCDInit_ILI9488();
#endif

#ifdef TFTLCD_ILI9327
	LCDInit_ILI9327();
#endif

#ifdef TFTLCD_ST7793
	LCDInit_ST7793();
#endif

#ifdef TFTLCD_NT35510
	LCDInit_NT35510();
#endif

#ifdef TFTLCD_ILI9486
	LCDInit_ILI9486();
#endif

#ifdef TFTLCD_ILI9481
	LCDInit_ILI9481();
#endif

	LCD_Display_Dir(TFTLCD_DIR);  //0：竖屏  1：横屏  默认竖屏
	LCD_Clear(lcdColor_BLACK);	  //用背景颜色清屏
}


//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//endx,endy:窗口右下角坐标
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t endx,uint16_t endy)
{
#ifdef TFTLCD_HX8357D
	LCD_WriteCmd(0x2A);
    LCD_WriteData(sx>>8);
    LCD_WriteData(sx&0XFF);
    LCD_WriteData(endx>>8);
    LCD_WriteData(endx&0XFF);

    LCD_WriteCmd(0x2b);
    LCD_WriteData(sy>>8);
    LCD_WriteData(sy&0XFF);
    LCD_WriteData(endy>>8);
    LCD_WriteData(endy&0XFF);
    LCD_WriteCmd(0x2c);
#endif

#ifdef TFTLCD_HX8352C
	LCD_WriteCmd(0x02);		LCD_WriteData(sx/256);
	LCD_WriteCmd(0x03);		LCD_WriteData(sx%256);
	LCD_WriteCmd(0x04);		LCD_WriteData(endx/256);
	LCD_WriteCmd(0x05);		LCD_WriteData(endx%256);

	LCD_WriteCmd(0x06);		LCD_WriteData(sy/256);
	LCD_WriteCmd(0x07);		LCD_WriteData(sy%256);
	LCD_WriteCmd(0x08);		LCD_WriteData(endy/256);
	LCD_WriteCmd(0x09);		LCD_WriteData(endy%256);

	LCD_WriteCmd(0x22);
#endif

#ifdef TFTLCD_ILI9488
	LCD_WriteCmd(0x2a);
	LCD_WriteData(sx/256);
	LCD_WriteData(sx%256);
	LCD_WriteData(endx/256);
	LCD_WriteData(endx%256);
	
	LCD_WriteCmd(0x2b);
	LCD_WriteData(sy/256);
	LCD_WriteData(sy%256);
	LCD_WriteData(endy/256);
	LCD_WriteData(endy%256);

	LCD_WriteCmd(0x2C);

#endif

#ifdef TFTLCD_ILI9327
	LCD_WriteCmd(0x2A);
	LCD_WriteData(sx/256);
	LCD_WriteData(sx%256);
	LCD_WriteData(endx/256);
	LCD_WriteData(endx%256);
	
	LCD_WriteCmd(0x2B);
	LCD_WriteData(sy/256);
	LCD_WriteData(sy%256);
	LCD_WriteData(endy/256);
	LCD_WriteData(endy%256);

	LCD_WriteCmd(0x2C);

#endif

#ifdef TFTLCD_ST7793
	if(LcdPara.dir==0)
	{
		LCD_WriteCmd(0x0210);
	    LCD_WriteData(sx);
		LCD_WriteCmd(0x0211);
	    LCD_WriteData(endx);
		LCD_WriteCmd(0x0212);
	    LCD_WriteData(sy);
		LCD_WriteCmd(0x0213);
	    LCD_WriteData(endy);
	
		LCD_WriteCmd(0x0200);
	    LCD_WriteData(sx);
		LCD_WriteCmd(0x0201);
	    LCD_WriteData(sy);
	}
	else
	{
		LCD_WriteCmd(0x0212);
	    LCD_WriteData(sx);
		LCD_WriteCmd(0x0213);
	    LCD_WriteData(endx);
		LCD_WriteCmd(0x0210);
	    LCD_WriteData(sy);
		LCD_WriteCmd(0x0211);
	    LCD_WriteData(endy);
	
		LCD_WriteCmd(0x0201);
	    LCD_WriteData(sx);
		LCD_WriteCmd(0x0200);
	    LCD_WriteData(sy);
	}
	LCD_WriteCmd(0x0202);
#endif

#ifdef TFTLCD_NT35510
	LCD_WriteCmd(0X2A00);LCD_WriteData(sx>>8);
	LCD_WriteCmd(0X2A01);LCD_WriteData(sx&0XFF);
	LCD_WriteCmd(0X2A02);LCD_WriteData(endx>>8);
	LCD_WriteCmd(0X2A03);LCD_WriteData(endx&0XFF);
	LCD_WriteCmd(0X2B00);LCD_WriteData(sy>>8);
	LCD_WriteCmd(0X2B01);LCD_WriteData(sy&0XFF);
	LCD_WriteCmd(0X2B02);LCD_WriteData(endy>>8);
	LCD_WriteCmd(0X2B03);LCD_WriteData(endy&0XFF);
	LCD_WriteCmd(0x2C00);
#endif

#ifdef TFTLCD_ILI9486
	LCD_WriteCmd(0x2A);
	LCD_WriteData(sx/256);
	LCD_WriteData(sx%256);
	LCD_WriteData(endx/256);
	LCD_WriteData(endx%256);
	
	LCD_WriteCmd(0x2B);
	LCD_WriteData(sy/256);
	LCD_WriteData(sy%256);
	LCD_WriteData(endy/256);
	LCD_WriteData(endy%256);

	LCD_WriteCmd(0x2C);
#endif

#ifdef TFTLCD_ILI9481
	LCD_WriteCmd(0x2A);
	LCD_WriteData(sx/256);
	LCD_WriteData(sx%256);
	LCD_WriteData(endx/256);
	LCD_WriteData(endx%256);

	LCD_WriteCmd(0x2B);
	LCD_WriteData(sy/256);
	LCD_WriteData(sy%256);
	LCD_WriteData(endy/256);
	LCD_WriteData(endy%256);

	LCD_WriteCmd(0x2C);
#endif

}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	uint16_t i, j ;
	LCD_Set_Window(0, 0, LcdPara.width-1, LcdPara.height-1);	 //作用区域
	for(i=0; i<LcdPara.width; i++)
		for (j=0; j<LcdPara.height; j++)
			LCD_WriteData_Color(color);
}

//清除yStart到yEnd的行
void LCD_ClearLine(uint16_t yStart, uint16_t yEnd, uint16_t color)
{
	uint16_t x, y ;
	LCD_Set_Window(0, yStart, LcdPara.width-1, yEnd);	 //作用区域
	for(x=0; x<LcdPara.width; x++)
		for (y=yStart; y<=yEnd; y++)
			LCD_WriteData_Color(color);
}


//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{          
	uint16_t temp;

    if((xStart > xEnd) || (yStart > yEnd))
    {
        return;
    }   
	LCD_Set_Window(xStart, yStart, xEnd, yEnd);
    xStart = xEnd - xStart + 1;
	yStart = yEnd - yStart + 1;

	while(xStart--)
	{
	 	temp = yStart;
		while (temp--)
	 	{			
			LCD_WriteData_Color(color);	
		}
	}	
} 

//画点
//x,y:坐标
//FRONT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_Set_Window(x, y, x, y);  //设置点的位置
	LCD_WriteData_Color(LcdFRONT_COLOR);
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_DrawFRONT_COLOR(uint16_t x,uint16_t y,uint16_t color)
{	   
	LCD_Set_Window(x, y, x, y);
	LCD_WriteData_Color(color);	
}

//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	if(x>=LcdPara.width||y>=LcdPara.height)return 0;	//超过了范围,直接返回
	LCD_Set_Window(x, y, x, y);

#ifdef TFTLCD_HX8357D
	LCD_WriteCmd(0X2E);
 	r=LCD_ReadData();								//dummy Read
 	r=LCD_ReadData();  		  						//实际坐标颜色
#endif

#ifdef TFTLCD_HX8352C
	LCD_WriteCmd(0X22);
 	r=TFTLCD->LCD_DATA;
	r=LCD_ReadData();
#endif

#ifdef TFTLCD_ILI9488
	LCD_WriteCmd(0X2e);
	r=LCD_ReadData();								//dummy Read
	r=LCD_ReadData();								//实际坐标颜色
	//printf("r=%X\r\n",r);
	b=LCD_ReadData();
	g=LCD_ReadData();
	//printf("g=%X\r\n",g);
	//printf("b=%X\r\n",b);
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
#endif

#ifdef TFTLCD_ILI9327
	LCD_WriteCmd(0X2e);
	r=LCD_ReadData();								//dummy Read
	r=LCD_ReadData();								//实际坐标颜色
	r=(r<<8)|(r>>8);
	return r;
#endif

#ifdef TFTLCD_ST7793
	if(x>=LcdPara.width||y>=LcdPara.height)return 0;	//超过了范围,直接返回
	LCD_Set_Window(x, y, x, y);
	LCD_WriteCmd(0x0202);
 	r=LCD_ReadData();  		  						//实际坐标颜色
#endif

#ifdef TFTLCD_NT35510
	LCD_WriteCmd(0X2E00);
	r=LCD_ReadData();								//dummy Read
	r=LCD_ReadData();								//实际坐标颜色

	b=LCD_ReadData();
	g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8;

	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
#endif

#ifdef TFTLCD_ILI9486
	LCD_WriteCmd(0X2e);
	r=LCD_ReadData();								//dummy Read
	r=LCD_ReadData();								//实际坐标颜色
	//printf("r=%X\r\n",r);
	b=LCD_ReadData();
	g=LCD_ReadData();
	//printf("g=%X\r\n",g);
	//printf("b=%X\r\n",b);
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
#endif

#ifdef TFTLCD_ILI9481
	LCD_WriteCmd(0X2e);
	r=LCD_ReadData();								//dummy Read
	r=LCD_ReadData();								//实际坐标颜色
	//printf("r=%X\r\n",r);
	b=LCD_ReadData();
	g=LCD_ReadData();
	//printf("g=%X\r\n",g);
	//printf("b=%X\r\n",b);
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
#endif

 	return r;
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawFRONT_COLOR(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

// 画一个十字的标记
// x：标记的X坐标
// y：标记的Y坐标
// color：标记的颜色
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t i;

    /* 画点 */
    LCD_Set_Window(x-1, y-1, x+1, y+1);
    for(i=0; i<9; i++)
    {
		LCD_WriteData_Color(color);   
    }

    /* 画竖 */
    LCD_Set_Window(x-4, y, x+4, y);
    for(i=0; i<9; i++)
    {
		LCD_WriteData_Color(color); 
    }

    /* 画横 */
    LCD_Set_Window(x, y-4, x, y+4);
    for(i=0; i<9; i++)
    {
		LCD_WriteData_Color(color); 
    }
}

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

//设置字体大小，只影响后面的显示
void	LCD_SetFontSize(uint8_t	fontSize)//	设置字体大小，只能是12，16，或24
{
	switch(fontSize)
	{
	case	lcdFont_Size12:
		LCD_FS=fontSize;
		LCD_SP10=15;	//1.0倍行距
		LCD_SP15=22;	//1.5倍行距
		LCD_SP20=30;	//2.0倍行距
		break;

	case	lcdFont_Size16:
		LCD_FS=fontSize;
		LCD_SP10=20;	//1.0倍行距
		LCD_SP15=30;	//1.5倍行距
		LCD_SP20=40;	//2.0倍行距
		break;

	case	lcdFont_Size24:
		LCD_FS=fontSize;
		LCD_SP10=30;	//1.0倍行距
		LCD_SP15=40;	//1.5倍行距
		LCD_SP20=50;	//2.0倍行距
	}
}

//在指定位置显示一个字符
//x,y:起始坐标
//charCode: 要显示的字符的ASCII码:" "--->"~"
//fontSize:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t charCode,uint8_t mode)
{
	uint8_t fontSize=LCD_FS;	//LCD_FS是全局字体大小变量
	LCD_CurX=x+fontSize/2;	//自动更新坐标LCD_CurX
	LCD_CurY=y;				//自动更新坐标LCD_CurY

    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(fontSize/8+((fontSize%8)?1:0))*(fontSize/2);		//得到字体一个字符对应点阵集所占的字节数
 	charCode=charCode-' ';	//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0; t<csize; t++)
	{   
		if(fontSize==12)
			temp=ascii_1206[charCode][t]; 	//调用1206字体
		else if(fontSize==16)
			temp=ascii_1608[charCode][t];	//调用1608字体
		else if(fontSize==24)
			temp=ascii_2412[charCode][t];	//调用2412字体
		else
			return;		//没有的字库

		for(t1=0;t1<8;t1++)
		{			    
			if(temp & 0x80)
				LCD_DrawFRONT_COLOR(x,y,LcdFRONT_COLOR);
			else if(mode==0)
				LCD_DrawFRONT_COLOR(x,y,LcdBACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LcdPara.height)
				return;		//超区域了
			if((y-y0)==fontSize)
			{
				y=y0;
				x++;
				if(x>=LcdPara.width)
					return;	//超区域了
				break;
			}
		}  	 
	}

}   
//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
		result*=m;
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
//void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
//{
//	uint8_t t,temp;
//	uint8_t enshow=0;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
//				continue;
//			}else enshow=1;
//
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
//	}
//}

//显示整数,传递进来的书num可以是 int8_t, int16_t, int32_t
void LCD_ShowInt(uint16_t x,uint16_t y,int32_t num)
{
	char buf[20];
	siprintf(buf,"%ld",num);	//<stdio.h>中的函数，转换为字符串,自动加'\0'
	LCD_ShowStr(x,y,(uint8_t*)buf);
}

//显示无符号整数,传递进来的数num可以是 uint8_t, uint16_t, uint32_t
void LCD_ShowUint(uint16_t x,uint16_t y,uint32_t num)
{
	char buf[20];
	siprintf(buf,"%lu",num);	//转换为字符串,自动加'\0'
	LCD_ShowStr(x,y,(uint8_t*)buf);
}

//十六进制显示无符号整数
// show0X 非零时显示前导符 "0x"
void LCD_ShowUintHex(uint16_t x,uint16_t y,uint32_t num, uint8_t show0X)
{
	char buf[20];
	if (show0X)
		siprintf(buf,"0x%lX",num);		//转换为字符串,自动加'\0'
	else
		siprintf(buf,"%lX",num);		//转换为字符串,自动加'\0'

	LCD_ShowStr(x,y,(uint8_t*)buf);
}

//显示无符号整数,传递进来的书num可以是 uint8_t, uint16_t, uint32_t, 参数digiCount表示数字个数
void LCD_ShowUintX0(uint16_t x,uint16_t y,uint32_t num, uint8_t digiCount)
{
	char buf[20];
	siprintf(buf,"%lu",num);	//转换为字符串,自动加'\0'

	uint8_t  len=strlen(buf);	//<string.h>中的函数
	if (len>=digiCount)	//如果实际长度大于需要显示的位数，直接显示
	{
		LCD_ShowStr(x,y,(uint8_t*)buf);
		return;
	}

	char buf2[20];
	uint8_t	i,j;
	for(i=0; i<(digiCount-len); i++)
		buf2[i]='0';	//前面用字符0填补

	j=digiCount-len;
	for(i=0; i<len; i++, j++)
		buf2[j]=buf[i];	//复制
	buf2[j]=0x00;	//结束符

	LCD_ShowStr(x,y,(uint8_t*)buf2);
}


//固定数字位数显示，前端补空格
void LCD_ShowUintX(uint16_t x,uint16_t y,uint32_t num, uint8_t digiCount)
{
	char buf[20];
	siprintf(buf,"%lu",num);	//转换为字符串,自动加'\0'

	uint8_t  len=strlen(buf);	//<string.h>中的函数
	if (len>=digiCount)	//如果实际长度大于需要显示的位数，直接显示
	{
		LCD_ShowStr(x,y,(uint8_t*)buf);
		return;
	}

	char buf2[20];
	uint8_t	i,j;
	for(i=0; i<(digiCount-len); i++)
		buf2[i]=' ';	//前面用字符0填补

	j=digiCount-len;
	for(i=0; i<len; i++, j++)
		buf2[j]=buf[i];	//复制
	buf2[j]=0x00;	//结束符

	LCD_ShowStr(x,y,(uint8_t*)buf2);
}
//显示浮点数，value是要显示的数值，decimals是小数点后位数个数
void LCD_ShowFloat(uint16_t x,uint16_t y,float value, uint8_t decimals)//显示浮点数
{
	char buf[20];
//1. 如果需要显示的小数位为零个，直接转换为整数后显示
	if (decimals==0)  //显示整数
	{
		long int intVal=lrintf(value);	// <math.h>中的函数，浮点数转换为长整型
		siprintf(buf,"%ld",intVal);	//无法直接转换浮点数,只能
		LCD_ShowStr(x,y,(uint8_t*)buf);
		return;
	}

//2. 判断正负数， 如value=-423.1763
	uint8_t  sign=0;	//正数
	if (value<0)	//负数
	{
		sign=1;	//负数
		value =-value;	//转换为正数value=423.1763
	}

//3. 四舍五入
	uint8_t i;
	float 	value10x=value;
	for(i=0; i<decimals; i++)
		value10x=10*value10x;	//乘以10X
	long int intFrac=roundf(value10x);	// <math.h>中的函数, 近似为整数,intFrac=42318

//4. 转换为倒序字符串，如 intFrac=42318, buf="81.324"
	uint8_t  len=0; //整数部分的长度
	uint8_t digi;
	while (1)
	{
		digi= intFrac % 10;	//先取余数，从后往前取
		intFrac=intFrac / 10; //整数除法，缩小10倍
		buf[len]=0x30+digi;  //倒序
		len++;
		if (len==decimals)
		{
			buf[len]='.';  //补充小数点
			len++;
		}

		if (intFrac<=0)
			break;
	}
	buf[len]=0x00;  //结束符

//5. 倒序复制
	char bufSeq[20];
	i=0;
	if (sign)
	{
		bufSeq[0]='-';
		i=1;
	}

	for (int8_t j=len-1; j>=0; j--,i++)
	{
		digi=buf[j];
		bufSeq[i]=digi;
	}
	bufSeq[i]=0x00;// 结束符
	LCD_ShowStr(x,y,(uint8_t*)bufSeq);
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
//void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
//{
//	uint8_t t,temp;
//	uint8_t enshow=0;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
//				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
// 				continue;
//			}else enshow=1;
//
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
//	}
//}


//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
//void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
void LCD_ShowStr(uint16_t x,uint16_t y,uint8_t *p)
{         
	uint8_t x0=x;
//	while((*p<='~')&&(*p>=' '))  //判断是不是非法字符,遇到'\0'自动结束
	while(*p!=0x00 && *p!='\n')  //遇到'\0'或'\n'自动结束
	{
		if(x>=LCD_W)	//移动到下一行
		{
			x =x0;
			y += LCD_FS;
		}
		if(y>=LCD_H) //超出屏幕区域
			break;	 //退出

		LCD_ShowChar(x,y,*p,SHOW_STR_MERGE);	//叠加模式（透明模式）， 非文字部分不用背景色画
//		LCD_ShowChar(x,y,*p,1);	//叠加模式（透明模式）， 非文字部分不用背景色画
//		LCD_ShowChar(x,y,*p,0);	//非叠加模式，非文字部分用背景色画，应该使用此方式，因为数字显示函数需要
		x += LCD_FS/2;
		p++;
	}
}

//显示字符串, 非叠加模式(不透明模式)，非文字部分用背景色画
//void LCD_ShowStrOpaque(uint16_t x,uint16_t y,uint8_t *p)
//{
//	uint8_t x0=x;
////	while((*p<='~')&&(*p>=' '))  //判断是不是非法字符,遇到'\0'自动结束
//	while(*p!=0x00 && *p!='\n')  //遇到'\0'或'\n'自动结束
//	{
//		if(x>=LCD_W)	//移动到下一行
//		{
//			x =x0;
//			y += LCD_FS;
//		}
//		if(y>=LCD_H) //超出屏幕区域
//			break;	 //退出
//
////		LCD_ShowChar(x,y,*p,1);	//叠加模式， 非文字部分不用背景色画
//		LCD_ShowChar(x,y,*p,0);	//非叠加模式(不透明模式)，非文字部分用背景色画
//		x += LCD_FS/2;
//		p++;
//	}
//}


/****************************************************************************
*函数名：LCD_ShowFontHZ
*输  入：x：汉字显示的X坐标
*      * y：汉字显示的Y坐标
*      * cn：要显示的汉字
*      * wordColor：文字的颜色
*      * backColor：背景颜色
*输  出：
*功  能：写二号楷体汉字
****************************************************************************/
#if 0
void LCD_ShowFontHZ(uint16_t x, uint16_t y, uint8_t *cn)
{  
	uint8_t i, j, wordNum;
	uint16_t color;
	while (*cn != '\0')
	{
		LCD_Set_Window(x, y, x+31, y+28);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)
			     &&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<116; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_WriteData_Color(LcdFRONT_COLOR);
						} 						
						else
						{
							LCD_WriteData_Color(LcdBACK_COLOR);
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 32;
	}
}
#endif


#if 1
void LCD_ShowFontHZ(uint16_t x, uint16_t y, uint8_t *cn)
{
	uint8_t i, j, wordNum;
	uint16_t color;
	uint16_t x0=x;
	uint16_t y0=y;
	while (*cn != '\0')
	{
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)
			     &&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<116; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_DrawFRONT_COLOR(x,y,LcdFRONT_COLOR);
						} 						
						else
						{
							LCD_DrawFRONT_COLOR(x,y,LcdBACK_COLOR);
						} 
						color<<=1;
						x++;
						if((x-x0)==32)
						{
							x=x0;
							y++;
							if((y-y0)==29)
							{
								y=y0;
							}
						}
					}//for(j=0;j<8;j++)结束
				}	
			}
			
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 32;
		x0=x;
	}
}	
#endif

//显示一幅完整的图片
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic)
{
	LCD_Set_Window(x, y, x+width-1, y+height-1); //设置LCD显示窗口

	uint16_t RGB565 = 0;
	uint32_t pos=0;
	for(uint16_t row=0; row<height; row++)
		for(uint16_t col=0; col<width; col++)
		{
			RGB565 = pic[pos + 1]; 	//读取高字节, 类似于小字节序存储的uin16_t数据
			RGB565 = RGB565 << 8;
			RGB565 = RGB565 | pic[pos];	//读取低字节
			LCD_WriteData_Color(RGB565);//逐点显示
			pos += 2;
		}
}

void LCD_ShowPartBackImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint8_t *pic)
{  //不一定整行或整列，需特别处理
	uint16_t	stopX=x+width-1;
	uint16_t	stopY=y+height-1;
	LCD_Set_Window(x, y, stopX, stopY);

	uint16_t RGBData = 0;	//像素颜色数据
	uint32_t DataPos;	//数据点在图片数组中的位置
	for (uint16_t j=y; j<=stopY; j++)	//像素点坐标,按行扫描
	{
		DataPos=(j*LCD_W+x)*2;
		for (uint16_t i=x; i<=stopX; i++)  //像素点坐标
		{
			RGBData = pic[DataPos + 1];
			RGBData = RGBData << 8;
			RGBData = RGBData | pic[DataPos];
			LCD_WriteData_Color(RGBData);//逐点显示
			DataPos += 2;
		}
	}
}

//显示16*16 点阵大小的汉字，
//使用HzLib.c里定义的GB2312汉字库  横向取模  16*16  显示任意汉字
#ifdef	USE_HZ_LIB
void LCD_ShowHZ16(uint16_t x, uint16_t y, uint8_t *str)
{
	uint8_t i,j;
	uint8_t buffer[32];
	uint16_t tmp_char=0;

	GetGBKCode(buffer,str);  /* 取字模数据 */

	for ( i = 0; i < 16; i++ )
	{
		tmp_char = buffer[i*2];
		tmp_char = ( tmp_char << 8 );
		tmp_char |= buffer[2*i+1];
		for (j = 0; j < 16; j++ )
		{
			if ( (tmp_char >> 15-j ) & 0x01 == 0x01 )
				LCD_DrawFRONT_COLOR(x+j,y+i,LcdFRONT_COLOR);  /* 字符颜色 */
			else
				LCD_DrawFRONT_COLOR(x+j,y+i,LcdBACK_COLOR);  /* 背景颜色 */
		}
	}

}
#endif


