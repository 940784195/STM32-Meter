#ifndef _tftlcd_H
#define _tftlcd_H
//作者王维波

//#include "system.h"
#include "main.h"


//定义LCD彩屏的驱动类型  可根据自己手上的彩屏背面型号来选择打开哪种驱动
//#define TFTLCD_HX8357D 

//#define TFTLCD_HX8352C		//3.6寸电阻屏,分辨率400*240，实验室

#define TFTLCD_ILI9488

//#define TFTLCD_ST7793

//#define TFTLCD_ILI9327

//#define TFTLCD_NT35510		//4.3寸电容屏，800*480

//#define TFTLCD_ILI9486		//3.5寸电阻屏的驱动芯片,480*320， 2019年买

//#define TFTLCD_ILI9481			//3.5寸电阻屏,分辨率480*320，2020年买

//#define		USE_HZ_LIB		//定义符号，是否使用汉字库,如果不使用就注释掉此行

#define TFTLCD_DIR	0	//0：竖屏  1：横屏  默认竖屏


//TFTLCD地址结构体
typedef struct
{
	uint16_t LCD_CMD;
	uint16_t LCD_DATA;
}TFTLCD_TypeDef;


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对齐! 111 1110=0X7E			    
#define TFTLCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)


//TFTLCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;		//LCD 高度
	uint16_t id;			//LCD ID
	uint8_t  dir;            //LCD 方向
}TFTLCD_ParaDef;

extern	uint16_t LCD_W;		//LCD 宽度,会在LCD_Display_Dir()函数里初始化
extern	uint16_t LCD_H;		//LCD 高度

extern	uint8_t	 	LCD_FS;		//fontsize, 一个字符宽度为半个字体大小
extern	uint16_t	LCD_SP10;	//1.0倍行距
extern	uint16_t	LCD_SP15;	//1.5倍行距
extern	uint16_t	LCD_SP20;	//2.0倍行距

extern	uint16_t 	LCD_CurX;	//当前位置X
extern	uint16_t 	LCD_CurY;	//当前位置Y

//LCD参数
extern TFTLCD_ParaDef LcdPara;	//管理LCD重要参数
//LCD的前端颜色和背景色
extern uint16_t  LcdFRONT_COLOR;//前端颜色 默认红色
extern uint16_t  LcdBACK_COLOR; //背景颜色.默认为白色

//字体大小定义
#define		lcdFont_Size12	0x0C		//字体12
#define		lcdFont_Size16	0x10		//字体16
#define		lcdFont_Size24	0x18		//字体24

//LCD_ShowStr()显示模式，1=融合模式，不清除背景；0=清除背景
#define		SHOW_STR_MERGE	0
#define		SHOW_STR_NMERGE 1

//画笔颜色
#define 	lcdColor_WHITE         	 0xFFFF
#define 	lcdColor_BLACK         	 0x0000
#define 	lcdColor_BLUE         	 0x001F
#define 	lcdColor_BRED            0XF81F
#define 	lcdColor_GRED 			 0XFFE0
#define 	lcdColor_GBLUE			 0X07FF
#define 	lcdColor_RED           	 0xF800
#define 	lcdColor_MAGENTA       	 0xF81F
#define 	lcdColor_GREEN         	 0x07E0
#define 	lcdColor_CYAN          	 0x7FFF
#define 	lcdColor_YELLOW        	 0xFFE0
#define 	lcdColor_BROWN 			 0XBC40 //棕色
#define 	lcdColor_BRRED 			 0XFC07 //棕红色
#define 	lcdColor_GRAY  			 0X8430 //灰色

/* LCD基本读写指令  */
void LCD_WriteCmd(uint16_t cmd);	//写指令
void LCD_WriteData(uint16_t data);	//写数据
void LCD_WriteCmdData(uint16_t cmd,uint16_t data);	//写指令数据
void LCD_WriteData_Color(uint16_t color);	//写颜色数据

//uint16_t LCD_ReadData_Color();	//获取当前点的颜色

//uint16_t LCD_ReadData(void);	//缺少这个函数，读数据

/*  LCD软件初始化 */
void TFTLCD_Init(void);           //初始化

/* LCD显示基本功能函数定义   */
//设置LCD显示方向，dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir);

//设置窗口(sx,sy)--(endx,endy),并自动设置画点坐标到窗口左上角(sx,sy),
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t endx,uint16_t endy);

void LCD_Clear(uint16_t Color);	 	//清屏

//清除yStart到yEnd的行
void LCD_ClearLine(uint16_t yStart, uint16_t yEnd, uint16_t color);

//单色填充一个矩形区域
void LCD_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);

//画点
void LCD_DrawPoint(uint16_t x,uint16_t y);//画点

//快速画点
void LCD_DrawFRONT_COLOR(uint16_t x,uint16_t y,uint16_t color);

//读取个某点的颜色值
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);

//画线
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

//用指定颜色画线
void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);

// 画一个十字的标记
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color);

//画矩形
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

//在指定位置画一个指定大小的圆
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);

//=========字符和数字显示=========
//在指定位置显示一个字符,字体大小只能是12,16,24这三种
void	LCD_SetFontSize(uint8_t	fontSize);//	设置字体大小，只能是12，16，或24

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t charCode,uint8_t mode);

//显示数字,高位为0,则不显示
//void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);

//以下5个函数是王维波定义的
void LCD_ShowInt(uint16_t x,uint16_t y,int32_t num);	//显示整数,实际数字长度
void LCD_ShowUint(uint16_t x,uint16_t y,uint32_t num);	//显示无符号整数
void LCD_ShowUintHex(uint16_t x,uint16_t y,uint32_t num, uint8_t show0X);	//十六进制显示无符号整数
void LCD_ShowUintX(uint16_t x,uint16_t y,uint32_t num, uint8_t digiCount);//固定数字位数显示，前端补空格
void LCD_ShowUintX0(uint16_t x,uint16_t y,uint32_t num, uint8_t digiCount);//固定数字位数显示，前端补0
void LCD_ShowFloat(uint16_t x,uint16_t y,float value, uint8_t decimals);//显示浮点数

//显示数字,高位为0,还是显示
//void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);

//显示字符串, 叠加模式(透明模式)，非字符部分不画点，用于在背景图片上显示文字
void LCD_ShowStr(uint16_t x,uint16_t y,uint8_t *p);
//void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);

//显示字符串, 非叠加模式(不透明模式)，非文字部分用背景色画
//void LCD_ShowStrOpaque(uint16_t x,uint16_t y,uint8_t *p);

//显示字符串, 非叠加模式（不透明模式），非字符部分画背景色
//void LCD_ShowStrOverlap(uint16_t x,uint16_t y,uint8_t *p);

//显示汉字，字模大小32*29，需自己做字模
void LCD_ShowFontHZ(uint16_t x, uint16_t y, uint8_t *cn);

//显示整个背景图片
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic);

//显示部分背景图片,(x,y,wide,high)是部分图片的坐标，pic是整幅背景图片数据
void LCD_ShowPartBackImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic);

//使用HzLib.c里定义的GB2312汉字库  横向取模  16*16  显示任意汉字
#ifdef	USE_HZ_LIB
	void LCD_ShowHZ16(uint16_t x, uint16_t y, uint8_t *str);
#endif

#endif
