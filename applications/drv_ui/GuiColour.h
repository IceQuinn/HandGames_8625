#ifndef __GUI_COLOUR_H__
#define __GUI_COLOUR_H__

#include <stdint.h>

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

#endif
