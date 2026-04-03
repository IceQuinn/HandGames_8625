/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-01-19     mutou       the first version
 */
#include "drv_UC1698u.h"
#include "drv_UC1698u_spi.h"



void LCD_Write_Data(void *Data, uint16_t len)
{
    LCD_RS_HIGH();         /* 数据高电平 */
    UC1698u_spi_Write(Data, len);
}

void LCD_Write_CMD(uint8_t Cmd)
{
    LCD_RS_LOW();    /* 命令低电平 */
    UC1698u_spi_Write(&Cmd, 1);
}




void Drv_UC1698u_Init(void)
{
    Drv_UC1698u_SPI_Init();
}
