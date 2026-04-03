/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-01-19     mutou       the first version
 */
#ifndef APPLICATIONS_UI_DRV_UC1698U_SPI_H_
#define APPLICATIONS_UI_DRV_UC1698U_SPI_H_

#include "drv_common.h"

#define LCD_CD              GET_PIN(C, 2)   // 数据/指令
#define LCD_CS              GET_PIN(C, 3)   // 片选
#define LCD_RST             GET_PIN(C, 1)   // 复位信号

void LCD_RS_LOW(void);
void LCD_RS_HIGH(void);

void Drv_UC1698u_SPI_Init(void);

void UC1698u_spi_Write(void* data, uint16_t len);

#endif /* APPLICATIONS_UI_DRV_UC1698U_SPI_H_ */
