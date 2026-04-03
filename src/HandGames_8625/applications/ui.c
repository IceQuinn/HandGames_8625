#include "ui.h"


void ui_thread(void *parameter)
{
    while (1)
    {
        GuiUpdateDisplayAll();
        rt_thread_mdelay(100);
    }
}