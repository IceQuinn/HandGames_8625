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


enum Btn_Key
{
    BTN_KEY_ESC = 0,
    BTN_KEY_UP,
    BTN_KEY_DOWN,
    BTN_KEY_ENTER,
    BTN_KEY_MAX,
};

typedef enum
{
    KEY_EVENT_NONE = 0,     // 无事件
    KEY_EVENT_CLICK,        // 点击
    KEY_EVENT_LONG,         // 长按
    KEY_EVENT_DOUBLE_CLICK, // 双击
    KEY_EVENT_MAX,
}KeyEvent;

typedef struct
{
    uint8_t state;          // 0: 无事件，1: 点击，2: 长按，3: 双击
    uint8_t cnt;            // 点击计数器
    uint16_t long_cnt;      // 长按计数器
    uint16_t double_cnt;    // 双击计数器
    uint8_t long_pressed;   // 长按标志
}Key;


#define KEY_SCAN_PERIOD     10     // 10ms扫描
#define KEY_DEBOUNCE        3      // 30ms消抖
#define KEY_LONG_TIME       100    // 1s长按
#define KEY_DOUBLE_TIME     20     // 500ms双击等待


Key key[KEY_EVENT_MAX];


uint16_t BTN_ESC    = GET_PIN(B, 1);
uint16_t BTN_UP     = GET_PIN(B, 0);
uint16_t BTN_DOWN   = GET_PIN(A, 1);
uint16_t BTN_ENTER  = GET_PIN(A, 0);

struct Btn_Key_Event_Str {
    KeyEvent Btn_Event;
    char *desc;
};

struct Btn_Key_Event_Str Btn_Key_Event[KEY_EVENT_MAX] = {
    {KEY_EVENT_NONE,            "无事件"},
    {KEY_EVENT_CLICK,           "点击"},
    {KEY_EVENT_LONG,            "长按"},
    {KEY_EVENT_DOUBLE_CLICK,    "双击"},
};

#define BTN_MOVE_LEFT_CLICK    (1 << 0)  // 左移点击
#define BTN_MOVE_RIGHT_CLICK   (1 << 1)  // 右移点击
#define BTN_ESC_CLICK          (1 << 2)  // 返回点击
#define BTN_CONFIRM_CLICK      (1 << 3)  // 确定点击
#define BTN_MOVE_LEFT_LONG     (1 << 4)  // 左移长按
#define BTN_MOVE_RIGHT_LONG    (1 << 5)  // 右移长按
#define BTN_ESC_LONG           (1 << 6)  // 返回长按
#define BTN_CONFIRM_LONG       (1 << 7)  // 确定长按
#define BTN_MOVE_LEFT_DOUBLE   (1 << 8)  // 左移双击
#define BTN_MOVE_RIGHT_DOUBLE  (1 << 9)  // 右移双击
#define BTN_ESC_DOUBLE         (1 << 10) // 返回双击
#define BTN_CONFIRM_DOUBLE     (1 << 11) // 确定双击


struct Btn_Key_Func_Str {
    uint32_t Btn_Key;
    char  *desc;
};

struct Btn_Key_Func_Str Btn_Key_Func[KEY_EVENT_MAX] = {
    {GET_PIN(B, 1),     "返回键"},
    {GET_PIN(B, 0),     "上键"},
    {GET_PIN(A, 1),     "下键"},
    {GET_PIN(A, 0),     "确定键"},
};


struct Btn_State_Table_Str
{
    uint32_t Btn_Key;
    KeyEvent KeyEvent;
};

struct Btn_State_Table_Str Btn_State_Table[KEY_EVENT_MAX] = {
    {GET_PIN(B, 1),     KEY_EVENT_NONE},
    {GET_PIN(B, 0),     KEY_EVENT_NONE},
    {GET_PIN(A, 1),     KEY_EVENT_NONE},
    {GET_PIN(A, 0),     KEY_EVENT_NONE},
};

uint8_t Key_ReadPin(uint8_t id)
{
    switch(id)
    {
        case BTN_KEY_ESC:
            return PIN_LOW == rt_pin_read(BTN_ESC); 
        case BTN_KEY_UP:
            return PIN_LOW == rt_pin_read(BTN_UP);
        case BTN_KEY_DOWN:
            return PIN_LOW == rt_pin_read(BTN_DOWN);
        case BTN_KEY_ENTER:
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
            if(val)
            {
                if(k->long_pressed == 0)
                {
                    k->cnt++;
                    if(k->cnt >= KEY_DEBOUNCE)
                    {
                        k->cnt = 0;
                        k->state = 1;
                        k->long_cnt = 0;
                        k->long_pressed = 0;
                    }
                }
                else
                    k->cnt = 0;
            }
            else
            {
                k->cnt = 0;
                k->long_pressed = 0;
            }
            break;

        case 1: // 按下中
            if(val)
            {
                k->long_cnt++;
                if(k->long_cnt >= KEY_LONG_TIME)
                {
                    k->state = 0;
                    k->long_cnt = 0;
                    k->long_pressed = 1;
                    return KEY_EVENT_LONG;
                }
            }
            else
            {
                k->cnt++;
                if(k->cnt >= KEY_DEBOUNCE)
                {
                    k->cnt = 0;
                    if(k->long_pressed == 0)
                    {
                        k->state = 2;
                        k->double_cnt = 0;
                    }
                    else
                        k->state = 0;
                }
            }
            break;

        case 2: // 等待双击
            if(val)
            {
                k->cnt++;
                if(k->cnt >= KEY_DEBOUNCE)
                {
                    k->cnt = 0;
                    k->state = 3;
                    k->long_cnt = 0;
                }
            }
            else
            {
                k->double_cnt++;
                if(k->double_cnt >= KEY_DOUBLE_TIME)
                {
                    k->state = 0;
                    k->double_cnt = 0;
                    return KEY_EVENT_CLICK;
                }
            }
            break;

        case 3: // 第二次按下中
            if(val)
            {
                k->long_cnt++;
                if(k->long_cnt >= KEY_LONG_TIME)
                {
                    k->state = 0;
                    k->long_cnt = 0;
                    k->long_pressed = 1;
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
                    k->long_pressed = 0;
                    return KEY_EVENT_DOUBLE_CLICK;
                }
            }
            break;
    }

    return KEY_EVENT_NONE;
}

/* 事件控制块 */
static struct rt_event Btn_event;











void Key_Task(void)
{
    for(int i=0;i<BTN_KEY_MAX;i++)
    {
        KeyEvent e = Key_Scan(i);

        if(e != KEY_EVENT_NONE)
        {
            // uint32_t flag = 0;
            // switch(e)
            // {
                rt_kprintf("%s %s\n", Btn_Key_Func[i].desc, Btn_Key_Event[e].desc);
                // case KEY_EVENT_CLICK:
                //     flag = (1 << i);
                //     break;
                // case KEY_EVENT_LONG:
                //     flag = (1 << (i + 4));
                //     break;
                // case KEY_EVENT_DOUBLE_CLICK:
                //     flag = (1 << (i + 8));
                //     break;
                // default:
                //     break;
            // }
            // if(flag)
            // {
            //     // rt_event_send(&Btn_event, flag);
            // }
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



uint32_t Btn_Event_Wait(int32_t timeout)
{
//    union Btn_e_flg_UNION Btn_e_flg = {.Btn_e_flg = 0};
//    int32_t err_t = RT_EOK;
//
//    err_t = rt_event_recv(&Btn_event,
//            (BTN_MOVE_LEFT_CLICK | BTN_MOVE_RIGHT_CLICK | BTN_ESC_CLICK | BTN_CONFIRM_CLICK |
//             BTN_MOVE_LEFT_LONG | BTN_MOVE_RIGHT_LONG | BTN_ESC_LONG | BTN_CONFIRM_LONG |
//             BTN_MOVE_LEFT_DOUBLE | BTN_MOVE_RIGHT_DOUBLE | BTN_ESC_DOUBLE | BTN_CONFIRM_DOUBLE),
//            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
//            timeout,
//            &Btn_e_flg);
//    if(-RT_ETIMEOUT == err_t){
//        Btn_e_flg.Btn_timeout = 1;
//    }
//    return Btn_e_flg.Btn_e_flg;
}


void button_therad(void)
{
    int32_t err_t = RT_EOK;
    /* 初始化事件对象 */
    if(RT_EOK != rt_event_init(&Btn_event, "Btn_event", RT_IPC_FLAG_PRIO))
    {
        rt_kprintf("init Btn_event failed.\n");
    }
    btn_gpio_init();
    while(1)
    {
        Key_Task();
        rt_thread_mdelay(10);
    }

}
