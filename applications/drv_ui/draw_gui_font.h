#ifndef __DRAW_GUI_FONT_H__
#define __DRAW_GUI_FONT_H__

#include <stdint.h>


/* 字体对齐类型 */
typedef enum{
	FONT_LEFT,  /* 左对齐 */
	FONT_MID,   /* 居中 */
	FONT_RIGHT,	/* 右对齐 */
}FontFlag;

/* 字体结构 */
typedef struct{
	uint8_t *name;      /* 字体名称 */
	uint8_t swide;      /* ASCII取模宽 */
	uint8_t shigh;      /* ASCII取模高 */
	uint8_t dwide;      /* 汉字取模宽 */
	uint8_t dhigh;      /* 汉字取模高 */
	uint8_t *pZmBuff;   /* 存放单个字模buff */
	/* ASCII取模函数 */
	uint8_t (*getSfont)(uint8_t code1, uint8_t *pBuff);
	/* 汉字取模函数 */
	uint8_t (*getDfont)(uint8_t code1, uint8_t code2,uint8_t *pBuff);
}lkdFont;

/* 文本信息 */
typedef struct{
	uint16_t x;         /* 坐标 */
	uint16_t y;
	uint16_t wide;
	uint16_t high;
	uint16_t beginOffset;
	uint8_t wInterval;  /* 宽度间隔 */
	uint8_t hInterval;  /* 高度间隔 */
	uint8_t flag;       /* 0 正常显示 1反显 */
}fontTextInfo;


/* 12x12字体函数 */
void GuiFont12(uint16_t x, uint16_t y, uint8_t *str);
/* 12x12字体对齐显示函数 */
void GuiFont12Align(uint16_t x, uint16_t y,uint16_t wide, FontFlag flag,uint8_t *str);
/* 写文本 */
void GuiText(fontTextInfo *tInfo, uint8_t *str);
/* 写行文本 */
void GuiRowText(uint16_t x, uint16_t y,uint16_t wide, FontFlag flag,uint8_t *str);
/* 设置当前要使用的字体指针 */
void GuiFontSet(lkdFont *pFont);
/* 获取当前字体 */
lkdFont *GetCurrentFont(void);



#endif  
