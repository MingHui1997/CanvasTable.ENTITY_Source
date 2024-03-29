﻿#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

extern int CT_RunnerStatus;
extern int CT_sceneWidth;
extern int CT_sceneHeight;
extern int CT_GridWidth;
extern int CT_GridHeight;
extern int *CT_ContainerTile;
extern int CT_TileLayerStatusList[32];
extern int CT_ContainerTileLength;
extern int CT_ContainerMaxLength_Tile;
extern unsigned int *CT_ResourceArgList;
extern int *CT_ContainerSprite;
extern int CT_ContainerMaxLength_Sprite;
extern int CT_SpriteLayerStatusList[32];
extern int CT_ContainerSpriteLength;
extern int CT_Repaint;

//--------------------------------------------------------------------
// 新建场景对象
//--------------------------------------------------------------------
void CTscene_new(CTscene *scene)
{
    scene->width = 960; //场景宽度
    scene->height = 540; //场景高度
    scene->grid_width = 30; //场景网格宽度
    scene->grid_height = 30; //场景网格高度
    scene->tile_image[0] = 0; //瓦片图像组
    scene->type = -1; //类型
    scene->tile_map[0] = NULL; //瓦片地图层0
    scene->sprite_map = NULL; //sprite地图
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 场景进入
//--------------------------------------------------------------------
void scene_enter(CTscene *scene)
{
    CT_RunnerStatus = 0; //停止运行器
    CT_RunnerReset(); //运行器重置
    if(scene != NULL) //如果场景对象不为空
    {
        if(scene->type == -1) //如果对象是场景对象
        {
            int *ti = scene->tile_image; //传递瓦片图像列表;
            int tile_num = 0; //瓦片网格数
            void (*sprite_map)() = scene->sprite_map; //sprite地图
            CT_sceneWidth = scene->width; //场景宽度
            CT_sceneHeight = scene->height; //场景高度
            CT_GridWidth = scene->grid_width; //场景网格宽度
            CT_GridHeight = scene->grid_height; //场景网格高度
            tile_num = (CT_sceneWidth / CT_GridWidth) * (CT_sceneHeight / CT_GridHeight); //计算瓦片网格数
            if(CT_sceneWidth > 0 && CT_sceneHeight > 0 && CT_GridWidth > 0 && CT_GridHeight) //如果场景大小和网格大小大于0
            {
                if(CT_sceneWidth >= CT_GridWidth && CT_sceneHeight >= CT_GridHeight) //如果场景大小大于等于网格大小
                {
                    int i;
                    for(i=0;i<32;i++) //遍历瓦片地图
                    {
                        int *tm = scene->tile_map[i]; //传递瓦片地图数据
                        if(tm != NULL) //如果瓦片地图数据不为空
                        {
                            int tile_x = 0; //瓦片位置x
                            int tile_y = 0; //瓦片位置y
                            int i2;
                            for(i2=0;i2<tile_num;i2++) //遍历瓦片地图当前层
                            {
                                if(tile_y < CT_sceneHeight) //如果当前瓦片位置y小于场景高度
                                {
                                    if(tile_x < CT_sceneWidth) //如果当前瓦片位置x小于场景宽度
                                    {
                                        if(tm[i2] != 0) //如果当前瓦片地图块不为空
                                        {
                                            int ti_i = 0; //瓦片图像索引
                                            int ti_gx = -1; //瓦片图块位置x
                                            int ti_gy = 0; //瓦片图块位置y
                                            size_t ldata_i = ti[ti_i] * 5; //资源参数数据索引
                                            unsigned int ti_width = CT_ResourceArgList[ldata_i+2]; //当前瓦片图像宽度
                                            unsigned int ti_height = CT_ResourceArgList[ldata_i+3]; //当前瓦片图像高度
                                            int ti_gxn = ti_width / CT_GridWidth; //当前瓦片图像横向网格数
                                            int ti_gyn = ti_height / CT_GridHeight; //当前瓦片图像纵向网格数
                                            int ti_index = tm[i2]; //瓦片地图块索引
                                            int i3;
                                            for(i3=0;i3<ti_index;i3++) //遍历瓦片图块
                                            {
                                                if(ti_gy < ti_gyn) //如果当前瓦片图块位置y小于瓦片图像高度
                                                {
                                                    if(ti_gx < ti_gxn) //如果瓦片图块位置x小于瓦片图像宽度
                                                    {
                                                        ti_gx += 1; //下一个瓦片图块
                                                    }
                                                    if(ti_gx >= ti_gxn) //如果下一个瓦片图块位置x大于等于瓦片图像宽度
                                                    {
                                                        ti_gx = 0; //瓦片图块位置x
                                                        ti_gy += 1; //下一行瓦片图块
                                                    }
                                                }
                                                if(ti_gy >= ti_gyn) //如果下一个瓦片图块位置y大于瓦片图像高度
                                                {
                                                    ti_gx = 0; //瓦片图块位置x
                                                    ti_gy = 0; //瓦片图块位置y
                                                    ti_i ++; //切换下一个瓦片图像
                                                    ldata_i = ti[ti_i] * 5; //资源参数数据索引
                                                    ti_width = CT_ResourceArgList[ldata_i+2]; //当前瓦片图片宽度
                                                    ti_height = CT_ResourceArgList[ldata_i+3]; //当前瓦片图片高度
                                                    ti_gxn = ti_width / CT_GridWidth; //当前瓦片图片横向网格数
                                                    ti_gyn = ti_height / CT_GridHeight; //当前瓦片图片纵向网格数
                                                    if(ti_i > 255) //如果瓦片图块索引没有对应的瓦片图块
                                                    {
                                                        cprint("ERROR : Beyond range of tile image group.");
                                                    }
                                                }
                                            }
                                            CT_TileBuild(i,tile_x,tile_y,ti[ti_i],ti_gx,ti_gy); //创建瓦片
                                        }
                                    tile_x += CT_GridWidth; //下一个瓦片位置x
                                    }
                                    if(tile_x >= CT_sceneWidth) //如果下一个瓦片位置x大于等于场景宽度
                                    {
                                        tile_x = 0;
                                        tile_y += CT_GridHeight; //下一行瓦片位置y
                                    }
                                }
                                if(tile_y >= CT_sceneHeight) //如果下一行瓦片位置y大于场景高度
                                {
                                    tile_x = 0;
                                    tile_y = 0;
                                }
                            }
                        }
                    }
                    if(sprite_map != NULL) //如果sprite地图不为空
                    {
                        sprite_map(); //创建sprite地图
                    }
                }
                else
                {
                    cprint("ERROR : The scene size must be greater than the grid size.");
                }
            }
            else
            {
                cprint("ERROR : Unable to enter scene.");
            }
        }
        else
        {
            cprint("ERROR : The specified scene is not a scene object.");     
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite地图
//--------------------------------------------------------------------
void m(int x,int y,CTsprite *ctspr)
{
    if(ctspr->type == -2) //如果对象类型是sprite
    {
        int id = ctspr->id; //sprite id
        int image = ctspr->image; //sprite图像
        unsigned int image_w = 0; //图像宽度
        unsigned int image_h = 0; //图像高度
        int subimg_w = 0; //sprite子图像宽度
        int subimg_h = 0; //sprite子图像高度
        int layer = ctspr->layer; //sprite层
        int subimg = ctspr->subimg; //sprite默认子图像索引
        int subnum = ctspr->subnum; //sprite子图像数量
        int coll_t = ctspr->collision_type; //sprite碰撞类型
        int coll_e = ctspr->collision_edge; //sprite边缘碰撞
        int box_x = ctspr->cbox_x; //sprite碰撞盒位置x
        int box_y = ctspr->cbox_y; //sprite碰撞盒位置y
        int box_w = ctspr->cbox_w; //sprite碰撞盒宽度
        int box_h = ctspr->cbox_h; //sprite碰撞盒高度
        int box_cx = ctspr->cbox_cx; //sprite碰撞盒中心位置x
        int box_cy = ctspr->cbox_cy; //sprite碰撞盒中心位置y
        int box_r = ctspr->cbox_r; //sprite半径
        int color = ctspr->color; //sprite颜色
        int colora = ctspr->colora; //sprite透明度
        if(image != -1) //如果sprite有图像
        {
            size_t ldata_i = image * 5; //资源列表数据索引
            int res_type = CT_ResourceArgList[ldata_i+1]; //获取资源类型
            if(res_type == 0) //如果资源类型是图像
            {
                image_w = CT_ResourceArgList[ldata_i+2]; //sprite图像宽度
                image_h = CT_ResourceArgList[ldata_i+3]; //sprite图像高度
                subimg_w = image_w / subnum; //sprite子图像宽度
                subimg_h = image_h; //子图像高度
                if(box_x == 0 && box_y == 0 && box_w == 0 && box_h == 0) //如果没有设置矩形碰撞盒
                {
                    box_x = 0; //sprite碰撞盒位置x
                    box_y = 0; //sprite碰撞盒位置y
                    box_w = subimg_w; //sprite碰撞盒宽度/半径
                    box_h = subimg_h; //sprite碰撞盒高度
                }
                if(box_cx == 0 && box_cy == 0 && box_r == 0) //如果没有设置圆形碰撞盒
                {
                    box_cx = subimg_w / 2; //sprite碰撞盒位置x
                    box_cy = subimg_h / 2; //sprite碰撞盒位置y
                    box_r = subimg_w / 2; //sprite碰撞盒宽度/半径
                }
            }
            else
            {
                cprint("ERROR : The resource index corresponding to the resource file is not a image.");
            }
        }
        if(subnum >= 1 && subimg <= subnum - 1) //如果sprite子图像数量大于等于1
        {
            int conspr_i = CT_ContainerSpriteLength; //sprite容器的当前索引
            size_t data_i = conspr_i * 25; //sprite容器的当前数据索引
            //将sprite数据写入sprite容器
            CT_ContainerSprite[data_i] = 1;
            CT_ContainerSprite[data_i+1] = id;
            CT_ContainerSprite[data_i+2] = layer;
            CT_ContainerSprite[data_i+3] = x;
            CT_ContainerSprite[data_i+4] = y;
            CT_ContainerSprite[data_i+5] = image;
            CT_ContainerSprite[data_i+6] = subimg;
            CT_ContainerSprite[data_i+7] = subnum;
            CT_ContainerSprite[data_i+8] = subimg_w;
            CT_ContainerSprite[data_i+9] = subimg_h;
            CT_ContainerSprite[data_i+10] = x;
            CT_ContainerSprite[data_i+11] = y;
            CT_ContainerSprite[data_i+12] = coll_t;
            CT_ContainerSprite[data_i+13] = coll_e;
            CT_ContainerSprite[data_i+15] = box_x;
            CT_ContainerSprite[data_i+16] = box_y;
            CT_ContainerSprite[data_i+17] = box_w;
            CT_ContainerSprite[data_i+18] = box_h;
            CT_ContainerSprite[data_i+19] = box_cx;
            CT_ContainerSprite[data_i+20] = box_cy;
            CT_ContainerSprite[data_i+21] = box_r;
            CT_ContainerSprite[data_i+22] = color * 0x100 + colora;
            CT_TaskCreate(conspr_i,ctspr->main,ctspr->loop); //创建sprite子程序
            CT_ContainerSpriteLength ++; //sprite容器长度 +1
            if(CT_ContainerSpriteLength == CT_ContainerMaxLength_Sprite) //如果sprite容器长度等于sprite容器最大长度
            {
                CT_ContainerPreallocated_Sprite(CT_ContainerSpriteLength+100); //为sprite容器重新分配内存
            }
            CT_SpriteLayerStatusList[layer] = 1; //将当前sprite层的状态标记为1
        }
        else
        {
            cprint("ERROR : The sprite subimage is error.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 瓦片创建
//--------------------------------------------------------------------
void tile_create(int l,int x,int y,int res,int gx,int gy)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        unsigned int tix = CT_GridWidth * gx; //瓦片图像位置x
        unsigned int tiy = CT_GridHeight * gy; //瓦片图像位置y
        int tile_i = CT_ContainerTileLength; //tile容器索引
        if(res != -1) //如果有tile图像
        {
            int res_type = CT_ResourceArgList[res*5+1]; //获取资源类型
            if(res_type == 0) //如果资源类型是图像
            {
                int i;
                size_t data_i;
                for(i=0;i<CT_ContainerTileLength;i++) //遍历Tile容器
                {
                    data_i = i * 7; //tile数据索引
                    if(CT_ContainerTile[data_i] == 0) //如果当前Tile为空
                    {
                        //将Tile数据写入Tile容器
                        CT_ContainerTile[data_i] = 1;
                        CT_ContainerTile[data_i+1] = l;
                        CT_ContainerTile[data_i+2] = x;
                        CT_ContainerTile[data_i+3] = y;
                        CT_ContainerTile[data_i+4] = res;
                        CT_ContainerTile[data_i+5] = tix;
                        CT_ContainerTile[data_i+6] = tiy;
                        CT_TileLayerStatusList[l] = 1; //将当前tile层的状态标记为1
                        CT_Repaint = 1; //请求重绘
                        return; //中断
                    }
                }
                data_i = tile_i * 7; //tile数据索引
                //将Tile数据写入Tile容器
                CT_ContainerTile[data_i] = 1;
                CT_ContainerTile[data_i+1] = l;
                CT_ContainerTile[data_i+2] = x;
                CT_ContainerTile[data_i+3] = y;
                CT_ContainerTile[data_i+4] = res;
                CT_ContainerTile[data_i+5] = tix;
                CT_ContainerTile[data_i+6] = tiy;
                CT_ContainerTileLength ++; //Tile容器索引 +1
                if(CT_ContainerTileLength == CT_ContainerMaxLength_Tile) //如果tile容器长度等于tile容器最大长度
                {
                    CT_ContainerPreallocated_Tile(CT_ContainerTileLength+300); //为tile容器重新分配内存
                }
                CT_TileLayerStatusList[l] = 1; //将当前tile层的状态标记为1
                CT_Repaint = 1; //请求重绘
            }
            else
            {
                cprint("ERROR : The resource index corresponding to the resource file is not a image.");
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 瓦片销毁
//--------------------------------------------------------------------
void tile_destroy(int l,int x,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int i;
        size_t data_i;
        for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器
        {
            data_i = i * 7; //tile数据索引
            if(CT_ContainerTile[data_i] != 0) //如果当前瓦片不为空
            {
                int tl = CT_ContainerTile[data_i+1]; //当前瓦片层
                if(tl == l) //如果当前瓦片层等于层
                {
                    int tx = CT_ContainerTile[data_i+2]; //当前瓦片位置x
                    int ty = CT_ContainerTile[data_i+3]; //当前瓦片位置y
                    if(x == tx && y == ty) //如果当前瓦片位置与参数位置相同
                    {
                        //清除当前瓦片数据
                        CT_ContainerTile[data_i] = 0; 
                        CT_ContainerTile[data_i+1] = 0;
                        CT_ContainerTile[data_i+2] = 0;
                        CT_ContainerTile[data_i+3] = 0;
                        CT_ContainerTile[data_i+4] = 0;
                        CT_ContainerTile[data_i+5] = 0;
                        CT_ContainerTile[data_i+6] = 0;
                        CT_Repaint = 1; //请求重绘
                        for(i=CT_ContainerTileLength-1;i>=-1;i--) //反向遍历tile容器
                        {
                            if(i >= 0) //如果tile索引大于等于0
                            {
                                data_i = i * 7; //tile数据索引
                                if(CT_ContainerTile[data_i] != 0) //如果当前tile不为空
                                {
                                    CT_ContainerTileLength = i + 1; //确定tile容器长度
                                    break; //中断
                                }
                            }
                            else
                            {
                                CT_ContainerTileLength = 0; //tile容器长度为0
                            }
                        }
                        break; //中断
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 瓦片检查
//--------------------------------------------------------------------
int tile_check(int l,int x,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int i;
        size_t data_i;
        for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器
        {
            data_i = i * 7; //tile数据索引
            if(CT_ContainerTile[data_i] != 0) //如果当前瓦片不为空
            {
                int tl = CT_ContainerTile[data_i+1]; //当前瓦片层
                if(tl == l) //如果当前瓦片层等于层
                {
                    int tx = CT_ContainerTile[data_i+2]; //当前瓦片位置x
                    int ty = CT_ContainerTile[data_i+3]; //当前瓦片位置y
                    int gw = CT_GridWidth; //场景网格宽度
                    int gh = CT_GridHeight; //场景网格高度
                    if(x >= tx && y >= ty && x < tx + gw && y < ty + gh) //如果当前位置有瓦片
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景宽度
//--------------------------------------------------------------------
int scene_width()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_sceneWidth; //返回场景宽度
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景高度
//--------------------------------------------------------------------
int scene_height()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_sceneHeight; //返回场景高度
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景网格宽度
//--------------------------------------------------------------------
int grid_width()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_GridWidth; //返回场景网格宽度
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景网格高度
//--------------------------------------------------------------------
int grid_height()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_GridHeight; //返回场景网格高度
    }
    return 0;
}
//--------------------------------------------------------------------