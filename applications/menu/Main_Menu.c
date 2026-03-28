#include "Main_Menu.h"
#include "lkdGui.h"










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

void Main_Menu_Init2(void)
{
	#define MENUSTACK_NUM 8
	MenuStack userMenuStack[MENUSTACK_NUM];
	
	//2. 定义二级菜单
	lkdMenuNode Node3_3 = {6, "no1-3", NULL, NULL,NULL };     // 信息
	lkdMenuNode Node2_2 = {5, "no1-2", &Node3_3, NULL,NULL };     // 信息
	lkdMenuNode Node1_1 = {4, "no1-1", &Node2_2, NULL,NULL };  // 参数设置
	
	//3. 定义一级菜单
	lkdMenuNode Node3 = {3, "no2", NULL, NULL,NULL };     // 信息
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

	defaultFontInit();

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

void Main_menu_Thread(void *param)
{
    Main_Menu_Init2();
    while(1)
    {
        // GuiWinDisplay();

        rt_thread_mdelay(100);

    }
}
