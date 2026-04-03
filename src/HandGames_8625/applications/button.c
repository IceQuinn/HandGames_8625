/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-03-06     mutou       the first version
 */
#include "button.h"

#include "drv_common.h"



uint8_t Key_ReadPin(uint8_t id);

#define KEY_LEFT    0
#define KEY_RIGHT   1
#define KEY_ROTATE  2
#define KEY_DROP    3

#define KEY_NUM     4

typedef enum
{
    KEY_EVENT_NONE = 0,
    KEY_EVENT_PRESS,
    KEY_EVENT_RELEASE,
    KEY_EVENT_LONG,
}KeyEvent;

typedef struct
{
    uint8_t state;
    uint8_t cnt;
    uint16_t long_cnt;
}Key;


#define KEY_SCAN_PERIOD     10     // 10ms扫描
#define KEY_DEBOUNCE        3      // 30ms消抖
#define KEY_LONG_TIME       100    // 1s长按


Key key[KEY_NUM];


uint16_t BTN_ESC    = GET_PIN(B, 1);
uint16_t BTN_UP     = GET_PIN(B, 0);
uint16_t BTN_DOWN   = GET_PIN(A, 1);
uint16_t BTN_ENTER  = GET_PIN(A, 0);

uint8_t Key_ReadPin(uint8_t id)
{
    switch(id)
    {
        case KEY_LEFT:
            return PIN_LOW == rt_pin_read(BTN_UP);

        case KEY_RIGHT:
            return PIN_LOW == rt_pin_read(BTN_DOWN);

        case KEY_ROTATE:
            return PIN_LOW == rt_pin_read(BTN_ESC);

        case KEY_DROP:
            return PIN_LOW == rt_pin_read(BTN_ENTER);
    }

    return 0;
}



KeyEvent Key_Scan(uint8_t id)
{
    Key *k = &key[id];

    uint8_t val = Key_ReadPin(id);

    switch(k->state)
    {
        case 0: // 松开
        {
            if(val)
            {
                k->cnt++;

                if(k->cnt >= KEY_DEBOUNCE)
                {
                    k->cnt = 0;
                    k->state = 1;
                    k->long_cnt = 0;
                    return KEY_EVENT_PRESS;
                }
            }
            else
            {
                k->cnt = 0;
            }
        }
        break;

        case 1: // 按下
        {
            if(val)
            {
                k->long_cnt++;

                if(k->long_cnt > KEY_LONG_TIME)
                {
                    k->long_cnt = 0;
                    return KEY_EVENT_LONG;
                }
            }
            else
            {
                k->cnt++;

                if(k->cnt >= KEY_DEBOUNCE)
                {
                    k->cnt = 0;
                    k->state = 0;
                    return KEY_EVENT_RELEASE;
                }
            }
        }
        break;
    }

    return KEY_EVENT_NONE;
}

extern void MenuLeft(void);
extern void MenuRight(void);
extern void MenuEsc(void);
extern void MenuConfirm(void);



void Key_Task(void)
{
    for(int i=0;i<KEY_NUM;i++)
    {
        KeyEvent e = Key_Scan(i);

        if(e == KEY_EVENT_PRESS)
        {
            switch(i)
            {
                case KEY_LEFT:
                    // MoveLeft();
                    MenuLeft();
                    break;

                case KEY_RIGHT:
                    // MoveRight();
                    MenuRight();
                    break;

                case KEY_ROTATE:
                    // Rotate();
                    MenuEsc();
                    break;

                case KEY_DROP:
                    // DropFast();
                    MenuConfirm();
                    break;
            }
        }
    }
}


void btn_gpio_init(void)
{
    rt_pin_mode(BTN_ENTER,  PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BTN_UP,     PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BTN_DOWN,   PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BTN_ESC,    PIN_MODE_INPUT_PULLUP);
}


void button_therad(void)
{
    btn_gpio_init();
    while(1)
    {
        Key_Task();
        rt_thread_mdelay(10);
    }

}
