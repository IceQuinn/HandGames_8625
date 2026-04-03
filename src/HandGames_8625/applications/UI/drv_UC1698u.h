/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-01-19     mutou       the first version
 */
#ifndef APPLICATIONS_UI_DRV_UC1698U_H_
#define APPLICATIONS_UI_DRV_UC1698U_H_

#include "drv_common.h"

void LCD_Write_Data(void *Data, uint16_t len);
void LCD_Write_CMD(uint8_t Cmd);

void Drv_UC1698u_Init(void);

#endif /* APPLICATIONS_UI_DRV_UC1698U_H_ */
