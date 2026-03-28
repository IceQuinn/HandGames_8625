/*
 * Copyright (c) 2006-2026, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-01-19     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "cfg_UC1698u.h"
#include "drv_common.h"

//#include "draw_gui_basic.h"
#include "lkdGui.h"

#include "button.h"
#include "fk.h"

//#include "draw_gui_font.h"
#include "ui.h"


#define countof(x)      (sizeof(x)/sizeof(x[0]))

/* 线程结构体 */
typedef struct
{
    char        *name;                                  /**< the name of thread */
    void        (*entry)(void *parameter);              /**entry the entry function of thread*/
    void        *parameter;                             /**parameter the parameter of thread enter function*/
    rt_uint32_t stack_size;                             /**stack_size the size of thread stack*/
    rt_uint8_t  priority;                               /**priority the priority of thread*/
    rt_uint32_t tick;                                   /**tick the time slice if there are same priority thread*/
}user_thread_table_typedef;

const user_thread_table_typedef user_thread_table[] = {
        {"measure",     thread_fk,  RT_NULL,    1024,   17, 15},    /* 计量线程 */
        {"meas_data",   button_therad,     RT_NULL,    1024,   22, 15},    /* 测量数据线程 */
//
//        {"record",      Record_Wave_Thread, RT_NULL,    1024,   6,  15},    /* 录波线程 */
//        {"event",       Event_Deal_Loop,    RT_NULL,    1024,   30, 10},    /* 事件记录线程 */
//        {"btn",         btn_thread_entry,   RT_NULL,    768,    9,  10},    /* 按键线程 */
       {"ui",          ui_thread,            RT_NULL,    2048,   26, 10},    /* 显示线程 */

//        {"relay",       relay_thread_entry, RT_NULL,    768,    8, 10},    /* 保护+交流量计算线程 */
//        {"md_send",     send_thread_entry,  RT_NULL,    1280,   29, 10},    /* 转存数据线程 */
//
//        {"md_s_poll",   modbus_deal_thread, RT_NULL,    1024,   21, 10},    /* ModBus解析线程 */
//        {"temp",        user_temp_thread,      RT_NULL,    4096,   29, 10},       /* 温度检测线程 */
//        {"led_run",     User_Led_Thread_Entry, RT_NULL, 512,    25, 10},    /* LED灯运行线程 */
//        {"645_sl",      dlt645_slave_thread_entry, NULL, 2048,  20,  15},   /* 645解析线程 */

};

void user_thread_init(void)
{
    rt_thread_t tid1 = RT_NULL;
    for(uint16_t i=0; i<countof(user_thread_table); i++)
    {
        tid1 = rt_thread_create(user_thread_table[i].name,
                user_thread_table[i].entry,
                user_thread_table[i].parameter,
                user_thread_table[i].stack_size,
                user_thread_table[i].priority,
                user_thread_table[i].tick);
        if (tid1 != RT_NULL)
        {
            rt_thread_startup(tid1);
        }
        else
        {
            LOG_E("%s thread create fail", user_thread_table[i].name);
        }
    }
}

int main(void)
{
    UC1698u_Init();

    // for (int var = 0; var < 160; ++var) {
    //     GuiDrawPoint(var, var, 1);
    // }
    for (int var = 0; var < 160; ++var) {
        GuiDrawPoint(var, 0, 1);
    }
    for (int var = 0; var < 160; ++var) {
        GuiDrawPoint(0, var, 1);
    }

    for (int var = 0; var < 160; ++var) {
        GuiDrawPoint(var, 159, 1);
    }
    for (int var = 0; var < 160; ++var) {
        GuiDrawPoint(159, var, 1);
    }

    GuiHPointLine(0, 8, 159, 1, 1);
    GuiRPointLine(101, 0, 159, 1, 1);

    // defaultFontInit();
    userAppPortInit();

//    GuiRowText(130, 20, 32, FONT_LEFT, "菜单");


    // GuiUpdateDisplayAll();

    // user_thread_init();

//    thread_fk();

    return RT_EOK;
}
