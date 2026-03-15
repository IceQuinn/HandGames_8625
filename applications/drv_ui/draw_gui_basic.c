#include "draw_gui_basic.h"
#include "cfg_UC1698u.h"


#define  GUIXMAX	160
#define  GUIYMAX	160

uint8_t const GetBit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/**
  *@brief 画水平点线
  *@param  x0,y0 起始坐标
  *@param  x1 结束坐标
  *@param  interval 每隔N点画一个点
  *@param  color 颜色
  *@retval None
  */
void GuiHPointLine(uint16_t x0, uint16_t y0, uint16_t x1, uint8_t interval, uint16_t color)
{
	uint16_t tx;
	
	if(x0 > x1){//检查,交换-> x0 < x1
		tx = x1;
		x1 = x0;
		x0 = tx;
	}
	
	/* 坐标检查 */
	if(y0 < 0 || y0 >= GUIYMAX){
		return;
	}
	if(x1 < 0){
		return;
	}
	if(x0 >= GUIXMAX){
		return;
	}
	if(x1 >= GUIXMAX){
		x1 = GUIXMAX - 1;
	}
	if(x0 < 0){
		x0 = 0;
	}
	
	/* 画线 */
	if(x0 == x1){
		GuiDrawPoint(x0, y0, color);
		return;
	}
	while(1){
		GuiDrawPoint(x0, y0, color);
		x0 += interval;
		if(x0 > x1){
			break;
		}
	}
}


/**
  *@brief 画垂直点线
  *@param  x0,y0 起始坐标
  *@param  y1 结束坐标
  *@param  interval 每隔N点画一个点
  *@param  color 颜色
  *@retval None
  */
void  GuiRPointLine(uint16_t x0, uint16_t y0, uint16_t y1,uint8_t interval, uint16_t color)
{
	uint16_t ty;
	if(y0 > y1){//检查,交换-> y0 < y1
		ty = y1;
		y1 = y0;
		y0 = ty;
	}
	
	/* 坐标检查 */
	if(x0 < 0 || x0 >= GUIXMAX){
		return;
	}
	if(y1 < 0){
		return;
	}
	if(y0 >= GUIYMAX){
		return;
	}
	if(y1 >= GUIYMAX){
		y1 = GUIYMAX - 1;
	}
	if(y0 < 0){
		y0 = 0;
	}
	
	/* 画线 */
	if(y0 == y1){
		GuiDrawPoint(x0, y0, color);
		return;
	}
	while(1){
		GuiDrawPoint(x0, y0, color);
		y0 += interval;
		if(y0 > y1){
			break;
		}
	}
}


/**
  *@brief 画位图
  *@param  bitMap 位图数组
  *@param  x0,y0 起始坐标
  *@param  xlen,ylen 宽度和高度
  *@param  flag 0 正常显示 1反显
  *@retval None
  */
void GuiBitMap(uint8_t *bitMap,uint16_t x0,\
	uint16_t y0,uint32_t xlen,uint32_t ylen, uint8_t flag)
{
	uint8_t z,zMax;
	uint16_t colour;
	uint32_t i,j,tj,tx,eachRow;
	
	if(x0 < 0 || y0 < 0 || x0 >= GUIXMAX || y0 >= GUIYMAX){
		return;
	}
	/* 列像素单位为字节 */
	if(xlen%8 != 0){
		eachRow = xlen/8 + 1;
	}
	else{
		eachRow = xlen/8;
	}
	/* 检查坐标最大，超过不显示 */
	if(x0 + xlen >= GUIXMAX){
		xlen = GUIXMAX - x0 - 1;
	}
	if(y0 + ylen >= GUIYMAX){
		ylen = GUIYMAX - y0 - 1;
	}
	/* 画图 */
	for(j = 0; j < ylen; j++){
		tj = eachRow * j;
		zMax = 8;
		for(i = 0; i < xlen; i += 8){
			if(i + 8 >= xlen){
				zMax = xlen - i;
			}
			tx = i/8;
			for(z = 0; z < zMax; z++){//位处理
				colour = (bitMap[tj + tx]&GetBit[z])?1:0;
				if(flag){//反显处理
					colour = colour?0:1;
				}
				GuiDrawPoint(x0 + i + z,y0 + j,colour);
			}
		}
	}
}
