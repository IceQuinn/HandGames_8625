/**
  * @file   lkdGui.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2019/06/20
  * @brief  lkdGui使用头文件
  */

#ifndef __LKDGUI_H
#define	__LKDGUI_H

#include "lkdGuiTypedef.h"

/* 总行数y 总列数 x */
#define  GUIXMAX	GUI_LCM_XMAX
#define  GUIYMAX	GUI_LCM_YMAX

 /* 画点 */
void GuiPoint(uint16_t x, uint16_t y, uint16_t color);
/* 画斜线 */
void GuiBiasLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t color);
/* 画水平线 */
void GuiHLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t color);
/* 画垂直线 */
void GuiRLine(uint16_t x0, uint16_t y0, uint16_t y1, uint16_t color);
/* 画水平点线 */
void GuiHPointLine(uint16_t x0, uint16_t y0, uint16_t x1, uint8_t interval, uint16_t color);
/* 画垂直点线 */
void GuiRPointLine(uint16_t x0, uint16_t y0, uint16_t y1, uint8_t interval, uint16_t color);
/* 画矩形 */
void GuiRect(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t color);
/* 画填充矩形 */
void GuiFillRect(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t color);
/* 画位图 */
void GuiBitMap(uint8_t *bitMap,uint16_t x0, uint16_t y0,uint32_t xlen,uint32_t ylen,uint8_t flag);
/* 画自定义位图 */
void GuiAreaBitMap(lkdBitmap *bitMap,uint16_t x0, uint16_t y0, uint16_t endx, uint16_t endy, uint8_t flag);
/* 画自定义位图,协议使用 */
void GuiAreaBitMapNumber(lkdBitmap *bitMap,uint8_t number,\
	uint16_t x0, uint16_t y0, uint16_t endx, uint16_t endy, uint8_t flag);

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

/* 交换前景色和背景色 */
void GuiExchangeColor(void);
/* 获取前景色 */
uint16_t GuiGetForecolor(void);
/* 获取背景色 */
uint16_t GuiGetbackcolor(void);
/* 设置背景色 */
void GuiSetbackcolor(uint16_t colour);
/* 设置前景色 */
void GuiSetForecolor(uint16_t colour);

/* 窗口初始化 */
void GuiWinInit(void);
/* 添加窗口 */
int8_t GuiWinAdd(lkdWin *pWindow);
/* 显示窗口 */
void GuiWinDisplay(void);
/* 删除顶层窗口 */
void GuiWinDeleteTop(void);
/* 获取顶层窗口 */
lkdWin *GuiGetTopWin(void);

/* 垂直滚动条 */
void GuiVScroll(lkdScroll *pScroll);
/* 水平滚动条 */
void GuiHScroll(lkdScroll *pScroll);
/* 按钮 */
void GuiButton(lkdButton *pButton);
/* 进度条 */
void GuiProGress(lkdProgress *pProg);


/* 菜单初始化 */
void GuiMenuInit(lkdMenu *pM);
/* 菜单选中节点向下移动 */
void GuiMenuItemDownMove(lkdMenu *pM);
/* 菜单选中节点向上移动 */
void GuiMenuItemUpMove(lkdMenu *pM);
/* 菜单当前节点展开 */
uint8_t GuiMenuCurrentNodeSonUnfold(lkdMenu *pM);
/* 菜单当前节点隐藏 */
uint8_t GuiMenuCurrentNodeHide(lkdMenu *pM);
/* 菜单重绘 */
void GuiMenuRedrawMenu(lkdMenu *pM);
/* 获取当前选中的节点 */
lkdMenuNode *GuiMenuGetCurrentpNode(lkdMenu *pM);
/* 获取当前选中的节点的ID */
uint16_t GuiMenuGetCurrentNodeID(lkdMenu *pM);
/* 菜单兄弟节点插入 */
void GuiMenuBroNodeInsert(lkdMenuNode *bNode, lkdMenuNode *pNode);
/* 菜单子节点插入 */
void GuiMenuSonNodeInsert(lkdMenuNode *fNode, lkdMenuNode *pNode);
/* 菜单节点删除 */
uint8_t GuiMenuNodeDelete(lkdMenuNode *fbNode, lkdMenuNode *thisNode);

/* 清全部屏幕 */
#define GuiClearScreen(color) GuiFillRect(0, 0, GUIXMAX - 1,GUIYMAX - 1,color);
/* 关显示 */
extern void CloseLcdDisplay(void);
/* 开显示 */
extern void OpenLcdDisplay(void);
/* 刷新缓存到lcd */
extern void GuiUpdateDisplayAll(void);
/* 刷新指定范围缓存到lcd */
extern void GuiRangeUpdateDisplay(uint16_t beginx, uint16_t beginy,uint16_t endx, uint16_t endy);
/* 画点到缓存 */
extern void GuiDrawPoint(uint16_t x, uint16_t y, uint16_t color);
/* 从硬件设备读点 */
extern void GuiReadPoint(uint16_t x, uint16_t y, uint16_t *pColor);

extern void GuiQRCodeBitMap(uint8_t *bitMap,uint16_t x0,
		uint16_t y0,uint32_t len, uint8_t flag);

void defaultFontInit(void);

#endif /* __LKDGUI_H */

/* END */
