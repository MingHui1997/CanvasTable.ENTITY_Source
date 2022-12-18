﻿#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>

extern int CT_DEBUG_MODE;
extern int CT_ViewX;
extern int CT_ViewY;
extern int CT_ViewLastX;
extern int CT_ViewLastY;
extern char **CT_ResourceList;
extern int **CT_ResourceArgList;
extern int CT_ResIndex;

//--------------------------------------------------------------------
// 控制台输出
//--------------------------------------------------------------------
void cprint(char *str)
{
    if(CT_DEBUG_MODE == 1) //如果调试模式开启
    {
        OutputDebugString(str); //将字符串输出到控制台
        OutputDebugString("\n");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取资源
//--------------------------------------------------------------------
int res(char *n)
{
    int i;
    for(i=0;i<CT_ResIndex;i++) //遍历资源列表
    {
        if(strcmp(CT_ResourceList[i],n) == 0) //如果找到指定资源名称
        {
            if(CT_ResourceArgList[i][0] == 1) //如果当前资源不为空
            {
                return i; //返回资源索引
            }
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 视野设置x
//--------------------------------------------------------------------
void view_set_x(int x)
{
    if(x != CT_ViewLastX) //如果x不等于上次x
    {
        CT_ViewX = x; //设置视野位置x
        CT_ViewLastX = x; //设置上次视野位置x
        CT_InterfaceRepaint(); //请求重绘
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置视野设置y
//--------------------------------------------------------------------
void view_set_y(int y)
{
    if(y != CT_ViewLastY) //如果y不等于上次y
    {
        CT_ViewY = y;//设置视野位置y
        CT_ViewLastY = y; //设置上次视野位置y
        CT_InterfaceRepaint(); //请求重绘
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 视野位置x
//--------------------------------------------------------------------
int view_x()
{
    return CT_ViewX; //返回视野位置x
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 视野位置y
//--------------------------------------------------------------------
int view_y()
{
    return CT_ViewY; //返回视野位置y
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// rgb颜色
//--------------------------------------------------------------------
int color(int r,int g,int b,int a)
{
    int red = 0x00; //red值
    int green = 0x00; //green值
    int blue = 0x00; //blue值
    int alpha = 0x00;
    int color_code = 0x00; //颜色码
    if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255 && a >= 0 && a <= 255) //如果参数r的范围在0 - 255之间
    {
        red = r * 0x1000000; //获取red值
        green += g * 0x10000; //计算green值
        blue += b * 0x100; //计算blue值
        alpha += a;
        color_code = red + green + blue + alpha; //获取颜色码
    }
    else
    {
        cprint("ERROR : The color value is error.");
    }
    return color_code;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 文本像素宽度
//--------------------------------------------------------------------
int string_pxwidth(char *str,int s)
{
    cprint("ERROR : This function is invalid in CanvasTable.ENTITY,please use function istring_pxwidth().");
    return 0;
}
//--------------------------------------------------------------------