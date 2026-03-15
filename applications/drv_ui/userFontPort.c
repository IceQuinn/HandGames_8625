/**
  * @file   userFontPort.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui字体驱动接口，由移植者填充函数的具体内容。
  */

// #include "include/lkdGui.h"
#include "draw_gui_font.h"
#include "GuiColour.h"

extern uint8_t GetFontASCII_8_16(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontGb2312_16_16(uint8_t, uint8_t, uint8_t*);

/* 字体控制实体 */
static lkdFont defaultFont;

/**
  *@brief 获取汉字字模
  *@param  code1,code2 汉字编码[H,L]
  *@param  pBuff 字模buff
  *@retval 0
  */
static uint8_t GetDfontData(uint8_t code1, uint8_t code2,uint8_t *pBuff)
{
    /* 添加用户代码 */
    GetFontGb2312_16_16(code1, code2, pBuff);
    return 0;
}

/**
  *@brief 获取ASCII字模
  *@param  code1, 编码
  *@param  pBuff 字模buff
  *@retval 0
  */
static uint8_t GetSfontData(uint8_t code1, uint8_t *pBuff)
{
    /* 添加用户代码 */
    GetFontASCII_8_16(code1, pBuff);
    return 0;
}

/**
  *@brief 字体初始化
  *@param  None
  *@retval None
  */
void defaultFontInit(void)
{
    /* 根据字体要求做相应的修改 */

    /* 此buff的大小由最大字模大小决定 */
	static uint8_t dataBuff[16*2];

	defaultFont.name = (uint8_t *)"汉字字模为16*16的GB2312,ASCII字模为8*16";
	defaultFont.dhigh = 16;
	defaultFont.dwide = 16;
	defaultFont.shigh = 16;
	defaultFont.swide = 8;
	defaultFont.pZmBuff = dataBuff;
	defaultFont.getDfont = GetDfontData;
	defaultFont.getSfont = GetSfontData;

    /* 设置为系统默认字体 */
	GuiFontSet(&defaultFont);
  GuiSetbackcolor(0);
  GuiSetForecolor(1);
}
