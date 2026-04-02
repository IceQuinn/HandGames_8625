#include "Main_Menu.h"
#include "lkdGui.h"
#include "drv_common.h"









static void UserMainWinFun(void *param)
 {
     //用户自定义代码。
    GuiRowText(0, 20,160, FONT_MID, "此窗口为主窗口");
    GuiUpdateDisplayAll();

    rt_thread_mdelay(1000);

    GuiRowText(0, 20,160, FONT_MID, "第二个窗口");
    GuiUpdateDisplayAll();

    rt_thread_mdelay(1000);
 }

/* 用户主窗口 */
lkdWin userMainWin = {
    .x = 10,
    .y = 10,
    .wide = 100,
    .hight = 100,
    .title = (uint8_t *)"主菜单",
    .param = NULL,
    .WindowFunction = UserMainWinFun,
};

void Main_Menu_Init(void)
{
    /* 1. 初始化lcd */

    /* 2. 初始化用户字体 */
    defaultFontInit();

    /* 3. 初始化窗口,添加窗口 */
    GuiWinInit();
    GuiWinAdd(&userMainWin);

    /* 4. 更新屏幕 */
//    GuiClearScreen(GuiGetbackcolor());
//    OpenLcdDisplay();
    GuiUpdateDisplayAll();
}

#define MENUSTACK_NUM 8
MenuStack userMenuStack[MENUSTACK_NUM];

//2. 定义二级菜单
lkdMenuNode Node3_3 = {6, "no1-3", NULL, NULL,NULL };     // 信息
lkdMenuNode Node2_2 = {5, "no1-2", &Node3_3, NULL,NULL };     // 信息
lkdMenuNode Node1_1 = {4, "no1-1", &Node2_2, NULL,NULL };  // 参数设置

//3. 定义一级菜单
lkdMenuNode Node3 = {3, "no3", NULL, NULL,NULL };     // 信息
lkdMenuNode Node2 = {2, "no2", &Node3, NULL,NULL };     // 信息
lkdMenuNode Node1 = {1, "no1", &Node2, &Node1_1,NULL };  // 参数设置

//3. 定义根菜单
lkdMenuNode NodeRoot0 = {0, "root", NULL, &Node1, NULL};

//初始化根节点
lkdMenu root={
    .x=0,
    .y=0,
    .wide=128,
    .hight=64,
    .ItemsWide=48,
    .Itemshigh=16,
    .index=1,//默认选中节点
    .stackNum=MENUSTACK_NUM,
    .stack = userMenuStack,
     .Root = &NodeRoot0
};

void Main_Menu_Init2(void)
{
	//初始化菜单
	GuiMenuInit(&root);
	//展开当前选中节点
	GuiMenuCurrentNodeSonUnfold(&root);
	
	
	GuiUpdateDisplayAll();/* 更新 */
    rt_thread_mdelay(2000);

    GuiMenuCurrentNodeSonUnfold(&root);
    GuiUpdateDisplayAll();/* 更新 */
    rt_thread_mdelay(2000);


    GuiMenuItemDownMove(&root);//选中下移
    GuiUpdateDisplayAll();/* 更新 */
    rt_thread_mdelay(2000);

}
union Menu_e_flg_UNION
{
    uint32_t Menu_e_flg;
    struct {
        uint32_t Menu_move_left       :1;     // 左移
        uint32_t Menu_move_right      :1;     // 右移
        uint32_t Menu_esc             :1;     // 返回
        uint32_t Menu_confirm         :1;     // 确定
        uint32_t Menu_timeout         :1;     // 超时
    };
};



void Main_Menu_Task(union Menu_e_flg_UNION *flag)
{
    if(flag->Menu_move_left)
    {
        rt_kprintf("Menu Move Left\n");
        // 处理菜单左移事件
        GuiMenuItemUpMove(&root);//选中上移
    }
    else if(flag->Menu_move_right){
        rt_kprintf("Menu Move Right\n");
        // 处理菜单右移事件
        GuiMenuItemDownMove(&root);//选中下移
    }
    else if(flag->Menu_esc){
        rt_kprintf("Menu ESC\n");
        // 处理菜单返回事件
        GuiMenuCurrentNodeHide(&root);
    }
    else if(flag->Menu_confirm){
        rt_kprintf("Menu Confirm\n");
        // 处理菜单确定事件
        GuiMenuCurrentNodeSonUnfold(&root);
    }
    else if(flag->Menu_timeout){
        // rt_kprintf("Menu Timeout\n");
        
        // 处理菜单超时事件
    }
    GuiUpdateDisplayAll();/* 更新 */
    // 处理菜单事件
}


/* 事件控制块 */
static struct rt_event Menu_event;



union Menu_e_flg_UNION Menu_e_flg;

#define MENU_MOVE_LEFT    (1 << 0)  // 左移//
#define MENU_MOVE_RIGHT   (1 << 1)
#define MENU_ESC          (1 << 2)
#define MENU_CONFIRM      (1 << 3)



void MenuLeft(void)
{
    rt_event_send(&Menu_event, MENU_MOVE_LEFT);
}

void MenuRight(void)
{
    rt_event_send(&Menu_event, MENU_MOVE_RIGHT);
}

void MenuEsc(void)
{
    rt_event_send(&Menu_event, MENU_ESC);
}

void MenuConfirm(void)
{
    rt_event_send(&Menu_event, MENU_CONFIRM);
}

void Main_menu_Thread(void *param)
{
    defaultFontInit();

    int32_t err_t = RT_EOK;
    /* 初始化事件对象 */
    if(RT_EOK != rt_event_init(&Menu_event, "Menu_event", RT_IPC_FLAG_PRIO))
    {
        rt_kprintf("init Menu_event failed.\n");
    }

    Main_Menu_Init2();
    while(1)
    {
        err_t = rt_event_recv(&Menu_event,
                (MENU_MOVE_LEFT|MENU_MOVE_RIGHT|MENU_ESC|MENU_CONFIRM),
                RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                700,
                &Menu_e_flg);

        if(-RT_ETIMEOUT == err_t){
            Menu_e_flg.Menu_timeout = 1;
        }

        Main_Menu_Task(&Menu_e_flg);
        Menu_e_flg.Menu_e_flg = 0;
        // GuiWinDisplay();

        rt_thread_mdelay(100);

    }
}
