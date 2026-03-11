/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-03-06     mutou       the first version
 */
#include "fk.h"
#include "drv_common.h"
#include <stdint.h>


// 地图数组
uint8_t map[150][100]; // 用于像素级碰撞检测


// 方块

const uint8_t blocks[7][4][4][4] =
{
/* I */
{
{{0,0,0,0},
 {1,1,1,1},
 {0,0,0,0},
 {0,0,0,0}},

{{0,0,1,0},
 {0,0,1,0},
 {0,0,1,0},
 {0,0,1,0}},

{{0,0,0,0},
 {1,1,1,1},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,0,0},
 {0,1,0,0},
 {0,1,0,0},
 {0,1,0,0}}
},

/* O */
{
{{0,1,1,0},
 {0,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,1,0},
 {0,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,1,0},
 {0,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,1,0},
 {0,1,1,0},
 {0,0,0,0},
 {0,0,0,0}}
},

/* T */
{
{{0,1,0,0},
 {1,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,0,0},
 {0,1,1,0},
 {0,1,0,0},
 {0,0,0,0}},

{{0,0,0,0},
 {1,1,1,0},
 {0,1,0,0},
 {0,0,0,0}},

{{0,1,0,0},
 {1,1,0,0},
 {0,1,0,0},
 {0,0,0,0}}
},

/* L */
{
{{0,0,1,0},
 {1,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,0,0},
 {0,1,0,0},
 {0,1,1,0},
 {0,0,0,0}},

{{0,0,0,0},
 {1,1,1,0},
 {1,0,0,0},
 {0,0,0,0}},

{{1,1,0,0},
 {0,1,0,0},
 {0,1,0,0},
 {0,0,0,0}}
},

/* J */
{
{{1,0,0,0},
 {1,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,1,0},
 {0,1,0,0},
 {0,1,0,0},
 {0,0,0,0}},

{{0,0,0,0},
 {1,1,1,0},
 {0,0,1,0},
 {0,0,0,0}},

{{0,1,0,0},
 {0,1,0,0},
 {1,1,0,0},
 {0,0,0,0}}
},

/* S */
{
{{0,1,1,0},
 {1,1,0,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,0,0},
 {0,1,1,0},
 {0,0,1,0},
 {0,0,0,0}},

{{0,1,1,0},
 {1,1,0,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,1,0,0},
 {0,1,1,0},
 {0,0,1,0},
 {0,0,0,0}}
},

/* Z */
{
{{1,1,0,0},
 {0,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,0,1,0},
 {0,1,1,0},
 {0,1,0,0},
 {0,0,0,0}},

{{1,1,0,0},
 {0,1,1,0},
 {0,0,0,0},
 {0,0,0,0}},

{{0,0,1,0},
 {0,1,1,0},
 {0,1,0,0},
 {0,0,0,0}}
}
};



#define BLOCK_SIZE 10


// 窗口大小控制
struct Size_Str
{
    uint16_t x;         // 窗口左上角x坐标
    uint16_t y;         // 窗口左上角y坐标
    uint16_t width;     // 窗口宽度 
    uint16_t height;    // 窗口高度
};

// 当前方块结构
typedef struct
{
    int type;   // 方块类型 0-6
    int rot;    // 旋转状态 0-3
    int x;      // 方块左上角x坐标（像素单位）
    int y;      // 方块左上角y坐标（像素单位）
}Tetris;

// Tetris C_Cube;

struct Cube_Str
{
    uint16_t type;   // 方块类型 0-6
    uint16_t rot;    // 旋转状态 0-3
    uint16_t x;      // 方块左上角x坐标（像素单位）
    uint16_t y;      // 方块左上角y坐标（像素单位）
};

struct Cube_Str C_Cube; // 当前方块

// 游戏窗口
struct Game_Window_Str
{
    // 游戏窗口
    struct Size_Str Size; // 窗口大小控制

    // 当前方块

    // 下一个方块
    struct Cube_Str Next_Cube;

    // 游戏信息
    
};

struct Tetris_Window_Str
{
    // 游戏窗口
    struct Game_Window_Str  Game_W; // 游戏窗口信息
    // 文本窗口
};


struct Tetris_Window_Str Tetris_W = {
    // 游戏窗口
    .Game_W.Size.x = 1,         // 留出1像素边框
    .Game_W.Size.y = 9,        // 留出10像素顶部空间显示分数等信息
    .Game_W.Size.width = 100,   
    .Game_W.Size.height = 150,
};


void Tetris_Draw_Point(int x, int y, int color)
{
    uint16_t basic_x = Tetris_W.Game_W.Size.x;
    uint16_t basic_y = Tetris_W.Game_W.Size.y;
    GuiDrawPoint(basic_x + x, basic_y + y, color);
}

// 绘制一个BLOCK_SIZE * BLOCK_SIZE方块
void DrawBlock(int x, int y, int color)
{
    for(int i=0;i<BLOCK_SIZE;i++)
    {
        for(int j=0;j<BLOCK_SIZE;j++)
        {
            Tetris_Draw_Point(x+j,y+i,color);
        }
    }
}




void DrawMap()
{
    for(int y=0;y<150;y++)
    {
        for(int x=0;x<100;x++)
        {
            Tetris_Draw_Point(x,y,map[y][x]);
        }
    }
}






// 绘制当前方块
void DrawCurrent(int color)
{
    // 16个方块，分别绘制
    for(int i=0;i<4;i++)
    {   // Y轴向方块绘制
        for(int j=0;j<4;j++)
        {   // X轴向方块绘制
            if(blocks[C_Cube.type][C_Cube.rot][i][j])
            {
                // 坐标单位为像素，转换为格子坐标后绘制
                DrawBlock(C_Cube.x + j*BLOCK_SIZE, C_Cube.y + i*BLOCK_SIZE, color);
            }
        }
    }

    // 绘制下一个方块
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            if(blocks[Tetris_W.Game_W.Next_Cube.type][Tetris_W.Game_W.Next_Cube.rot][i][j])
            {
                // 显示在窗口右侧
                DrawBlock(Tetris_W.Game_W.Next_Cube.x + j*BLOCK_SIZE, Tetris_W.Game_W.Next_Cube.y + i*BLOCK_SIZE, color);
            }
        }
    }
}

// 绘制当前方块
void DrawCurrent_To_Map(uint16_t x, uint16_t y, uint16_t color)
{
    // 10个方块，分别绘制
    for(int i=0;i<BLOCK_SIZE;i++)
    {   // Y轴向方块绘制
        for(int j=0;j<BLOCK_SIZE;j++)
        {   // X轴向方块绘制
            map[y + i][x + j] = color;
        }
    }
}

// 碰撞检测
int CheckCollision(int nx,int ny,int rot)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(blocks[C_Cube.type][rot][i][j])
            {
                // 寻找带颜色的方块，检查是否碰撞
                // 坐标单位为像素，转换为格子坐标
                int x = (nx + j*BLOCK_SIZE);
                int y = (ny + i*BLOCK_SIZE);

                // 左边界判断
                if(x<0)
                    return 1;

                // 右边界判断
                if((x + BLOCK_SIZE) > 10*BLOCK_SIZE)
                    return 1;

                // 下边界判断
                if((y) >= 16*BLOCK_SIZE)
                    return 1;

                // 和地图上的点检查碰撞
                for (size_t block_i = 0; block_i < BLOCK_SIZE; block_i++)
                {
                    if(map[y][x + block_i])
                        return 1;
                }
            }
        }
    }
    return 0;
}

// 固定方块
void FixBlock()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(blocks[C_Cube.type][C_Cube.rot][i][j])
            {
                // 坐标单位为像素，转换为格子坐标
                int x = C_Cube.x + j*BLOCK_SIZE;
                int y = C_Cube.y + i*BLOCK_SIZE;

                DrawCurrent_To_Map(x, y, 1);
            }
        }
    }
}

// 消行
void ClearLines()
{
    for(int y=140;y>=0;)
    {
        int full = 1;

        for(int x=0;x<100;x++)
        {
            if(map[y][x]==0)
            {
                full = 0;
                break;
            }
        }

        if(full)
        {
            // 将该行上方的所有行向下移动一行
            for(int yy=y;yy>0;)
            {
                for(int xx=0;xx<100;xx++)
                {
                    for (uint16_t block_i = 0; block_i < BLOCK_SIZE; block_i++)
                    {
                        map[yy+block_i][xx]=map[yy-BLOCK_SIZE+block_i][xx];// 上一行的数据复制到当前行
                    }
                }
                yy -= BLOCK_SIZE;
            }

            // 清空顶部第一行
            for(int x=0;x<100;x++)
                for (uint16_t block_i = 0; block_i < BLOCK_SIZE; block_i++){
                    map[block_i][x]=0;
                }
                

            y += BLOCK_SIZE;// 重新检查当前行（因为外层y--，所以y++抵消掉）
        }
        y -= BLOCK_SIZE;
    }
}

// 初始化方块
void Block_Init()
{
    C_Cube.type = rand()%7;
    C_Cube.rot  = 0;
    C_Cube.x    = 2 * BLOCK_SIZE;  // 转换为像素单位
    C_Cube.y    = 0;

    Tetris_W.Game_W.Next_Cube.type = rand()%7;
    Tetris_W.Game_W.Next_Cube.rot  = 0;
    Tetris_W.Game_W.Next_Cube.x    = 6 * BLOCK_SIZE; // 显示在窗口右侧
    Tetris_W.Game_W.Next_Cube.y    = 0;
}


// 传递方块，生成新方块
void NewBlock()
{
    C_Cube.type = Tetris_W.Game_W.Next_Cube.type;
    C_Cube.rot  = Tetris_W.Game_W.Next_Cube.rot;
    C_Cube.x    = 2 * BLOCK_SIZE;
    C_Cube.y    = 0;

    Tetris_W.Game_W.Next_Cube.type = rand()%7;
    Tetris_W.Game_W.Next_Cube.rot  = 0;
    Tetris_W.Game_W.Next_Cube.x    = 6 * BLOCK_SIZE; // 显示在窗口右侧
    Tetris_W.Game_W.Next_Cube.y    = 0;
}


void TetrisTask()
{
    // 先擦除当前方块,将带颜色的方块擦除掉
    DrawCurrent(0);
        
    // 方块下落时，的碰撞检测（下落一格）
    if(!CheckCollision(C_Cube.x, C_Cube.y+BLOCK_SIZE, C_Cube.rot))
    {
        C_Cube.y += BLOCK_SIZE;
    }
    else
    {
        // 固定方块
        FixBlock();
        ClearLines();
        NewBlock();
        
        // 检查新方块是否能放置（游戏结束判定）
        if(CheckCollision(C_Cube.x, C_Cube.y, C_Cube.rot))
        {
            // 方块到顶，重新开始游戏
            TetrisInit();
        }
    }

    DrawMap();
    DrawCurrent(1);
}







void TetrisInit()
{
    memset(map, 0, sizeof(map));
    Block_Init();
}



void MoveLeft(void)
{
    DrawCurrent(0);   // 擦除当前方块

    // 左移10像素（一个格子宽度）
    if(!CheckCollision(C_Cube.x-BLOCK_SIZE, C_Cube.y, C_Cube.rot))
    {
        C_Cube.x -= BLOCK_SIZE;
    }

    DrawCurrent(1);   // 重新绘制
}
void MoveRight(void)
{
    DrawCurrent(0);

    // 右移10像素（一个格子宽度）
    if(!CheckCollision(C_Cube.x+BLOCK_SIZE, C_Cube.y, C_Cube.rot))
    {
        C_Cube.x += BLOCK_SIZE;
    }

    DrawCurrent(1);
}

void Rotate(void)
{
    int new_rot = (C_Cube.rot + 1) % 4;

    DrawCurrent(0);

    if(!CheckCollision(C_Cube.x, C_Cube.y, new_rot))
    {
        C_Cube.rot = new_rot;
    }

    DrawCurrent(1);
}

void DropFast(void)
{
    DrawCurrent(0);

    // 快速下落（以格子为单位）
    while(!CheckCollision(C_Cube.x, C_Cube.y+BLOCK_SIZE, C_Cube.rot))
    {
        C_Cube.y += BLOCK_SIZE;
    }

    DrawCurrent(1);

    // 固定方块
    FixBlock();

    // 消行
    ClearLines();

    // 新方块
    NewBlock();

    // 检查新方块是否能放置（游戏结束判定）
    if(CheckCollision(C_Cube.x, C_Cube.y, C_Cube.rot))
    {
        // 方块到顶，重新开始游戏
        TetrisInit();
    }

    DrawMap();
    DrawCurrent(1);
}



//void button_entry()
//{
//    if(KEY_LEFT)  MoveLeft();
//
//    if(KEY_RIGHT) MoveRight();
//
//    if(KEY_UP)    Rotate();
//
//    if(KEY_DOWN)  MoveDown();
//
//    if(KEY_OK)    DropFast();
//}



void thread_fk(void)
{
    TetrisInit();
    while(1)
    {
        TetrisTask();
        GuiUpdateDisplayAll();
        rt_thread_mdelay(400);
    }

}



