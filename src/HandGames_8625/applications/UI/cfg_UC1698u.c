/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-01-20     mutou       the first version
 */
#include "cfg_UC1698u.h"
#include "drv_UC1698u.h"

#define  GUIXMAX    160
#define  GUIYMAX    160

// LCD配置
void cfg_UC1698u(void)
{
    LCD_Write_CMD(0xE2);     //system reset
    rt_thread_mdelay(200);

    LCD_Write_CMD(0xeb);    //set bias=12 这个需要根据你们具体的bias来更改
    LCD_Write_CMD(0x81);    //set vop
    LCD_Write_CMD(90);     //set vop=16.5v（72）, 15.8V（46）16.3V(64)这个参数需要根据你们的VOP来更改
    LCD_Write_CMD(0x2b);    //set internal pump control


    LCD_Write_CMD(0xc4);     //set lcd mapping  control

    LCD_Write_CMD(0x89);     //set ram address control      //原始数据


    //WriteLcdCommand(0xc2);     //set lcd mapping  control       //修改数据
    //WriteLcdCommand(0x8d);     //set ram address control


    LCD_Write_CMD(0x24);    // set TC=-0.00%
    LCD_Write_CMD(0xa2);    // set line rate

    LCD_Write_CMD(0xf1);    //set com end
    LCD_Write_CMD(159);



    LCD_Write_CMD(0x85);    //set partical display enable
    LCD_Write_CMD(0xf2);    //set partical display start
    LCD_Write_CMD(0);
    LCD_Write_CMD(0xf3);    //set partical display end
    LCD_Write_CMD(159);

    LCD_Write_CMD(0x40);    //set scorll line
    LCD_Write_CMD(0x50);    //set scroll line

    LCD_Write_CMD(0xd1);    //set color pattern
    LCD_Write_CMD(0xd5);    //set color mode 64k
    LCD_Write_CMD(0xC8);    //set n-line inversion
    LCD_Write_CMD(0x1a);
    LCD_Write_CMD(0xd8);    //set com scan function

    LCD_Write_CMD(0xAD);    //set display enable
}

// LCD配置显示窗口
void setWindowsProgame(void)
{

    LCD_Write_CMD(0x70);  //行地址0
    LCD_Write_CMD(0x60);
    LCD_Write_CMD(0x12);  //列地址35
    LCD_Write_CMD(0x03);

    LCD_Write_CMD(0xf4);
    LCD_Write_CMD(35);//start  col        左边界35
    LCD_Write_CMD(0xf6);
    LCD_Write_CMD(88);//end  col          右边界88
    LCD_Write_CMD(0xf5);
    LCD_Write_CMD(0);//start row          上边界0
    LCD_Write_CMD(0xf7);
    LCD_Write_CMD(159);//end row          下边界159
    LCD_Write_CMD(0xf8);  //窗口功能开

}

uint8_t GuiBuff[GUIYMAX][GUIXMAX/2+1];
void GuiUpdateDisplayAll(void)
{
    setWindowsProgame();
    LCD_Write_Data(GuiBuff, 160*81);

}

void GuiDrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    if(color == 0){//白色
        GuiBuff[159-y][(159-x) >> 1] &= ~(0x0f << (4*(x%2)));
    }
    else{//黑色
        GuiBuff[159-y][(159-x) >> 1] |= (0x0f << (4*(x%2)));
    }
}

void UC1698u_Init(void)
{
    Drv_UC1698u_Init();

    cfg_UC1698u();
}
