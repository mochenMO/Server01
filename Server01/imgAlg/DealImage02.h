#ifndef _DEALIMAGE_01_H_
#define _DEALIMAGE_01_H_

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>
#include<string.h>


// **************************************************************************************************
// 通用函数工具

namespace myImgAlg
{

unsigned char Tool_RBG(double BRRA)
{
    if (BRRA > 255)
        return (unsigned char)255;
    else if (BRRA < 0)
        return (unsigned char)0;
    else
        return (unsigned char)BRRA;
}


// 获取文件的后缀名
char* Tool_GetFlieExta(char* filename)
{
    int fileLen = strlen(filename);
    int exLen = 0;
    char fileExta[255];
    memset(fileExta, 0, sizeof(fileExta));

    for (int i = fileLen - 1; i > 0; i--)
        if (filename[i] == '.') {
            exLen = fileLen - i;
            break;
        }
    strncpy(fileExta, filename + fileLen - exLen, exLen);
    return fileExta;
}


// **************************************************************************************************
// 图像文件的读取与保存

// BGRA颜色结构体
typedef struct tagBGRA
{
    unsigned char blue;          // 该颜色的蓝色分量  (值范围为0-255)
    unsigned char green;         // 该颜色的绿色分量  (值范围为0-255)
    unsigned char red;           // 该颜色的红色分量  (值范围为0-255)
    unsigned char transparency;  // 透明度，在bmp中是保留值，无实际效果
}BGRA, * PBGRA;

// 图像结构体
typedef struct tagIMAGE
{
    unsigned int w;
    unsigned int h;
    BGRA* color;
}IMAGE, * PIMAGE;


// BMP文件的处理

// BMP文件头结构体
typedef struct tagBITMAP_HEAD_INFO
{
    /* bmp文件头的信息，有#的是重点!!*/
    // bmp文件头
    unsigned short  bfType;             // 0x424D，即BM字符串，表明是bmp格式文件
    unsigned int    bfSize;             // ###总的bmp文件大小 以字节为单位     
    unsigned short  bfReserved1;        // 保留，必须设置为0                     
    unsigned short  bfReserved2;        // 保留，必须设置为0 
    unsigned int    bfOffBits;          // ###总的bmp头部的大小（包括位图信息头），即到像素数据的偏移  
    // 位图信息头
    unsigned int    biSize;             // 位图信息头的大小
    unsigned int    biWidth;            // ###图像的宽  
    unsigned int    biHeight;           // ###图像的高  
    unsigned short  biPlanes;           // 颜色平面数，即调色盘数，恒等于1 
    unsigned short  biBitCount;         // ###图片颜色的位数,一般为32
    unsigned int    biCompression;      // 说明图象数据压缩的类型，0为不压缩
    unsigned int    biSizeImage;        // 像素数据所占大小,因为使用BI_RGB，所以设置为0
    unsigned int    biXPelsPerMeter;    // 说明水平分辨率，缺省为0
    unsigned int    biYPelsPerMeter;    // 说明垂直分辨率，缺省为0
    unsigned int    biClrUsed;          // 说明本位图实际使用调色盘的颜色索引数，0表示全部
    unsigned int    biClrImportant;     // 说明本位图重要调色盘的颜色索引数，0表示全都重要
}BITMAP_HEAD_INFO, * PBITMAP_HEAD_INFO;

// 加载BMP图片
IMAGE Image_bmp_load(char* filename)
{
    IMAGE im;
    BITMAP_HEAD_INFO bmpHeadInfo;
    FILE* fp;

    if ((fp = fopen(filename, "rb")) == NULL)
        printf("打开%s文件失败！\n", filename);
 
    // 读取bmp头部
    // bmp文件头
    fread(&bmpHeadInfo.bfType, 1, sizeof(bmpHeadInfo.bfType), fp);
    fread(&bmpHeadInfo.bfSize, 1, sizeof(bmpHeadInfo.bfSize), fp);
    fread(&bmpHeadInfo.bfReserved1, 1, sizeof(bmpHeadInfo.bfReserved1), fp);
    fread(&bmpHeadInfo.bfReserved2, 1, sizeof(bmpHeadInfo.bfReserved2), fp);
    fread(&bmpHeadInfo.bfOffBits, 1, sizeof(bmpHeadInfo.bfOffBits), fp);
    // 位图信息头
    fread(&bmpHeadInfo.biSize, 1, sizeof(bmpHeadInfo.biSize), fp);
    fread(&bmpHeadInfo.biWidth, 1, sizeof(bmpHeadInfo.biWidth), fp);
    fread(&bmpHeadInfo.biHeight, 1, sizeof(bmpHeadInfo.biHeight), fp);
    fread(&bmpHeadInfo.biPlanes, 1, sizeof(bmpHeadInfo.biPlanes), fp);
    fread(&bmpHeadInfo.biBitCount, 1, sizeof(bmpHeadInfo.biBitCount), fp);
    fread(&bmpHeadInfo.biCompression, 1, sizeof(bmpHeadInfo.biCompression), fp);
    fread(&bmpHeadInfo.biSizeImage, 1, sizeof(bmpHeadInfo.biSizeImage), fp);
    fread(&bmpHeadInfo.biXPelsPerMeter, 1, sizeof(bmpHeadInfo.biXPelsPerMeter), fp);
    fread(&bmpHeadInfo.biYPelsPerMeter, 1, sizeof(bmpHeadInfo.biYPelsPerMeter), fp);
    fread(&bmpHeadInfo.biClrUsed, 1, sizeof(bmpHeadInfo.biClrUsed), fp);
    fread(&bmpHeadInfo.biClrImportant, 1, sizeof(bmpHeadInfo.biClrImportant), fp);

    // 读取bmp位图数据
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * (bmpHeadInfo.biWidth * bmpHeadInfo.biHeight));
    fseek(fp, bmpHeadInfo.bfOffBits, SEEK_SET);

    if (bmpHeadInfo.biBitCount == 32)
    {
        for (unsigned int i = 0; i < bmpHeadInfo.biWidth * bmpHeadInfo.biHeight; i++)
            fread(&bgra[i], 1, sizeof(BGRA), fp);
    }
    else if (bmpHeadInfo.biBitCount == 24)
    {
        // 计算每行补几个字节零
        int k = 4 * (3 * bmpHeadInfo.biWidth / 4 + 1) - 3 * bmpHeadInfo.biWidth;
        for (unsigned int i = 0; i < bmpHeadInfo.biWidth * bmpHeadInfo.biHeight; i++)
        {
            if (k != 4 && (ftell(fp)- 54 + k ) % (3 * bmpHeadInfo.biWidth + k)==0)
                fseek(fp, ftell(fp) + k, SEEK_SET);

            fread(&bgra[i].blue, 1, sizeof(unsigned char), fp);
            fread(&bgra[i].green, 1, sizeof(unsigned char), fp);
            fread(&bgra[i].red, 1, sizeof(unsigned char), fp);
            bgra[i].transparency = (unsigned char)0xFF;
        }
    }

    im.color = bgra;
    im.w = bmpHeadInfo.biWidth;
    im.h = bmpHeadInfo.biHeight;

    fclose(fp);
    return im;
}

// 保存BMP图片
void Image_bmp_save(char* filename, IMAGE im)
{
    FILE* fp = fopen(filename, "wb");

    unsigned short  bfType = 0x4D42;                // 0x424D，即BM字符串，表明是bmp格式文件
    unsigned int    bfSize = im.w * im.h * 4 + 54;  // ###总的bmp文件大小 以字节为单位     
    unsigned short  bfReserved1 = 0;                // 保留，必须设置为0                     
    unsigned short  bfReserved2 = 0;                // 保留，必须设置为0 
    unsigned int    bfOffBits = 54;                 // ###总的bmp头部的大小（包括位图信息头），即到像素数据的偏移  
    unsigned int    biSize = 40;                    // 位图信息头的大小
    unsigned int    biWidth = im.w;                 // ###图像的宽  
    unsigned int    biHeight = im.h;                // ###图像的高  
    unsigned short  biPlanes = 1;                   // 颜色平面数，即调色盘数，恒等于1 
    unsigned short  biBitCount = 32;                // ###图片颜色的位数,一般为32
    unsigned int    biCompression = 0;              // 说明图象数据压缩的类型，0为不压缩
    unsigned int    biSizeImage = 0;                // 像素数据所占大小,因为使用BI_RGB，所以设置为0
    unsigned int    biXPelsPerMeter = 0;            // 说明水平分辨率，缺省为0
    unsigned int    biYPelsPerMeter = 0;            // 说明垂直分辨率，缺省为0
    unsigned int    biClrUsed = 0;                  // 说明本位图实际使用调色盘的颜色索引数，0表示全部
    unsigned int    biClrImportant = 0;             // 说明本位图重要调色盘的颜色索引数，0表示全都重要

    fwrite(&bfType, 2, 1, fp);
    fwrite(&bfSize, 4, 1, fp);
    fwrite(&bfReserved1, 2, 1, fp);
    fwrite(&bfReserved2, 2, 1, fp);
    fwrite(&bfOffBits, 4, 1, fp);
    fwrite(&biSize, 4, 1, fp);
    fwrite(&biWidth, 4, 1, fp);
    fwrite(&biHeight, 4, 1, fp);
    fwrite(&biPlanes, 2, 1, fp);
    fwrite(&biBitCount, 2, 1, fp);
    fwrite(&biCompression, 4, 1, fp);
    fwrite(&biSizeImage, 4, 1, fp);
    fwrite(&biXPelsPerMeter, 4, 1, fp);
    fwrite(&biYPelsPerMeter, 4, 1, fp);
    fwrite(&biClrUsed, 4, 1, fp);
    fwrite(&biClrImportant, 4, 1, fp);

    fwrite(im.color, sizeof(BGRA) * im.w * im.h, 1, fp);

    fclose(fp);
}


// JPG结构体
// typedef struct tagJPG
// 加载JPG图片
// IMAGE Image_jpg_load(char* filename)
// 保存JPG图片
// void Image_jpg_save(char* filename, IMAGE im)


// 加载图片
IMAGE Image_load(char* filename)
{
    IMAGE im;
    char* fileEx = Tool_GetFlieExta(filename);

    if (strcmp(fileEx, ".bmp") == 0)
        im = Image_bmp_load(filename);
    // else if (strcmp(fileEx, ".jpg") == 0)
        // im = Image_jpg_load(filename);

    return im;
}

// 保存图片
void Image_save(char* filename, IMAGE im)
{
    char* fileEx = Tool_GetFlieExta(filename);

    if (strcmp(fileEx, ".bmp") == 0)
        Image_bmp_save(filename, im);
    // else if (strcmp(fileEx, ".jpg") == 0)
        // im = Image_jpg_save(filename);
}

// 查看图片
void Image_show(char* filename)
{
    system(filename);
}

// 释放图像结构体
void Image_free(IMAGE im)
{
    free(im.color);
}


// **************************************************************************************************
// 图像的形状变换

// 图像的任意大小的缩放

// 缩放图片(最近邻插值法)(部分区域有明显的锯齿状，不推荐使用)
IMAGE Transform_shape_nearest(IMAGE im, unsigned int newWidth, unsigned int newHeight)
{
    // 算法核心：通过新图的坐标计算原图的坐标

    float fx, fy;    // f->former  原来（原图坐标）
    float dx, dy;    // d->Decimal 小数（原图坐标的小数部分）

    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * newWidth * newHeight);

    // 遍历整张新图
    for (unsigned int i = 0; i < newWidth * newHeight; i++)
    {
        // 通过新图的坐标计算原图的坐标
        fx = (i % newWidth) * ((float)im.w / newWidth);
        fy = (i / newWidth) * ((float)im.h / newHeight);

        // 计算原图坐标的小数部分
        dx = fx - (int)fx;
        dy = fy - (int)fy;

        fx = (dx <= 0.5 ? (int)fx : (int)fx + 1);
        fy = (dy <= 0.5 ? (int)fy : (int)fy + 1);

        unsigned int k = fx + fy * im.w; // 注意乘的是原图的w

        if (k >= im.w * im.h)  // 判断k是否越界
            k = im.w * im.h - 1;

        bgra[i].blue = im.color[k].blue;
        bgra[i].green = im.color[k].green;
        bgra[i].red = im.color[k].red;
        bgra[i].transparency = 255;
    }

    free(im.color);
    im.color = bgra;
    im.w = newWidth;
    im.h = newHeight;
    return im;
}

// 缩放图片(双线性插值法)(推荐使用)
IMAGE Transform_shape_linear(IMAGE im, unsigned int newWidth, unsigned int newHeight)
{
    float fx, fy, dx, dy;
    int k1, k2, k3, k4;

    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * newWidth * newHeight);

    for (unsigned int i = 0; i < newWidth * newHeight; i++)
    {
        // 通过新图的坐标计算原图的坐标
        fx = (i % newWidth) * ((float)im.w / newWidth);
        fy = (i / newWidth) * ((float)im.h / newHeight);

        dx = fx - (int)fx;
        dy = fy - (int)fy;

        fx = (int)fx;
        fy = (int)fy;

        // 分别计算四个角上点的坐标
        k1 = fx + fy * im.w;
        k2 = fx + 1 + fy * im.w;
        k3 = fx + (fy + 1) * im.w;
        k4 = fx + 1 + (fy + 1) * im.w;

        // 判断是否越界
        if (k1 >= im.w * im.h)
            k1 = im.w * im.h - 1;
        if (k2 >= im.w * im.h)
            k2 = im.w * im.h - 1;
        if (k3 >= im.w * im.h)
            k3 = im.w * im.h - 1;
        if (k4 >= im.w * im.h)
            k4 = im.w * im.h - 1;

        bgra[i].blue = (im.color[k1].blue * (1 - dx) + im.color[k2].blue * dx) * (1 - dy) + (im.color[k3].blue * (1 - dx) + im.color[k4].blue * dx) * dy;
        bgra[i].green = (im.color[k1].green * (1 - dx) + im.color[k2].green * dx) * (1 - dy) + (im.color[k3].green * (1 - dx) + im.color[k4].green * dx) * dy;
        bgra[i].red = (im.color[k1].red * (1 - dx) + im.color[k2].red * dx) * (1 - dy) + (im.color[k3].red * (1 - dx) + im.color[k4].red * dx) * dy;
        bgra[i].transparency = 255;
    }

    free(im.color);
    im.color = bgra;
    im.w = (int)newWidth;
    im.h = (int)newHeight;
    return im;
}

// 图像的任意角度的旋转
IMAGE Transform_shape_whirl(IMAGE im, float angle)
{
    // 转角度换成弧度
    angle = 3.141592 * angle / 180;

    float cosnum = (float)cos(angle);
    float sinnum = (float)sin(angle);

    // 计算原图的四个角的坐标
    int fx1 = 0;
    int fy1 = 0;
    int fx2 = im.w - 1;
    int fy2 = 0;
    int fx3 = 0;
    int fy3 = im.h - 1;
    int fx4 = im.w - 1;
    int fy4 = im.h - 1;

    // 计算旋转后的图像四个角的坐标
    int nx1 = 0;
    int ny1 = 0;
    int nx2 = (int)(fx2 * cosnum + fy2 * sinnum);
    int ny2 = (int)(fy2 * cosnum - fx2 * sinnum);
    int nx3 = (int)(fx3 * cosnum + fy3 * sinnum);
    int ny3 = (int)(fy3 * cosnum - fx3 * sinnum);
    int nx4 = (int)(fx4 * cosnum + fy4 * sinnum);
    int ny4 = (int)(fy4 * cosnum - fx4 * sinnum);

    // 计算旋转后的图像的宽和高
    unsigned int width = abs(max(max(nx1, nx2), max(nx3, nx4))) + abs(min(min(nx1, nx2), min(nx3, nx4))) + 1;
    unsigned int hight = abs(max(max(ny1, ny2), max(ny3, ny4))) + abs(min(min(ny1, ny2), min(ny3, ny4))) + 1;

    // 计算旋转后的图像到第一象限的位置偏移
    int bx = abs(min(min(nx1, nx2), min(nx3, nx4)));
    int by = abs(min(min(ny2, ny3), ny4));

    // 申请并初始化内存空间
    BGRA* bgra = (BGRA*)calloc(width * hight, sizeof(BGRA));

    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        // 注意(int)放的位置，不能随便移动,因为y可能为负的小数 
        int k = ((i % im.w) * cosnum + (i / im.w) * sinnum + bx) + (int)((i / im.w) * cosnum - (i % im.w) * sinnum + by) * width;

        // 判断是否越界
        if (k >= width * hight)
            k = width * hight - 1;

        bgra[k].blue = im.color[i].blue;
        bgra[k].green = im.color[i].green;
        bgra[k].red = im.color[i].red;
        bgra[k].transparency = 255;
    }

    // 用邻近的像素填充空白区域
    for (unsigned int i = 0; i < width * hight; i++)
    {
        if (bgra[i].transparency != 255 && bgra[i + 1].transparency == 255)
        {
            bgra[i].blue = bgra[i - 1].blue;
            bgra[i].green = bgra[i - 1].green;
            bgra[i].red = bgra[i - 1].red;
            bgra[i].transparency = 255;
        }
    }

    free(im.color);
    im.color = bgra;
    im.w = width;
    im.h = hight;
    return im;
}



#define UPTURN_MODE_HORIZONTAL 0    // 水平翻转
#define UPTURN_MODE_VERTICAL 1      // 垂直翻转
// 图像的镜像翻转
IMAGE Transform_shape_upturn(IMAGE im, int upturn_mode)
{
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);

    if (upturn_mode == UPTURN_MODE_HORIZONTAL)
        for (unsigned int i = 0; i < im.w * im.h; i++)
            bgra[i] = im.color[(im.w - 1 - (i % im.w)) + i / im.w * im.w];  // 水平翻转是y坐标不变，x坐标翻转
    else if (upturn_mode == UPTURN_MODE_VERTICAL)
        for (unsigned int i = 0; i < im.w * im.h; i++)
            bgra[i] = im.color[(i % im.w) + (im.h - 1 - i / im.w) * im.w]; // 垂直翻转是x坐标不变，y坐标翻转

    free(im.color); 
    im.color = bgra;
    return im;
}


// **************************************************************************************************
// 图像的颜色变换

#define GRAY_MODE_WEIGHT 1           // 加权法(推荐使用)
#define GRAY_MODE_BEST 2             // 最值法
#define GRAY_MODE_AVERAGE 3          // 均值法
#define GRAY_MODE_PART_RED 4         // 分量法_RED
#define GRAY_MODE_PART_GREEN 5       // 分量法_GREEN
#define GRAY_MODE_PART_BLUE 6        // 分量法_BLUE

// 灰度图
void Transform_color_grayscale(IMAGE im, int grayscale_mode)
{
    unsigned char color = 0;
    switch (grayscale_mode)
    {
    case GRAY_MODE_WEIGHT:
    {
        for (unsigned int i = 0; i < im.w * im.h; i++)
        {
            color = (im.color[i].blue * 114 + im.color[i].green * 587 + im.color[i].red * 299) / 1000;
            im.color[i].blue = color;
            im.color[i].green = color;
            im.color[i].red = color;
        }
        break;
    }

    case GRAY_MODE_BEST:
    {
        for (unsigned int i = 0; i < im.w * im.h; i++)
        {
            color = im.color[i].blue;
            if (color < im.color[i].red)
                color = im.color[i].red;
            if (color < im.color[i].green)
                color = im.color[i].green;
            im.color[i].blue = color;
            im.color[i].green = color;
            im.color[i].red = color;
        }
        break;
    }

    case GRAY_MODE_AVERAGE:
    {
        for (unsigned int i = 0; i < im.w * im.h; i++)
        {
            color = (im.color[i].blue + im.color[i].green + im.color[i].red) / 3;
            im.color[i].blue = color;
            im.color[i].green = color;
            im.color[i].red = color;
        }
        break;
    }

    case GRAY_MODE_PART_RED:
    {
        for (unsigned int i = 0; i < im.w * im.h; i++)
        {
            im.color[i].blue = im.color[i].red;
            im.color[i].green = im.color[i].red;
        }
        break;
    }

    case GRAY_MODE_PART_GREEN:
    {
        for (unsigned int i = 0; i < im.w * im.h; i++)
        {
            im.color[i].blue = im.color[i].green;
            im.color[i].red = im.color[i].green;
        }
        break;
    }

    case GRAY_MODE_PART_BLUE:
    {
        for (unsigned int i = 0; i < im.w * im.h; i++)
        {
            im.color[i].green = im.color[i].blue;
            im.color[i].red = im.color[i].blue;
        }
        break;
    }

    }
}


// 二值图(自定义阈值法)
void Transform_color_BW_DIY(IMAGE im, unsigned char Threshold)
{
    unsigned char color = 0;
    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        // 先转换成灰度图
        color = (im.color[i].blue * 114 + im.color[i].green * 587 + im.color[i].red * 299) / 1000;
        if (color >= Threshold) // Threshold的值在不同的图片中是不同的
            color = 255;
        else
            color = 0;

        im.color[i].blue = color;
        im.color[i].green = color;
        im.color[i].red = color;
    }
}


// 二值图(大津法OSTU，适用双峰直方图。当图像的整体颜色差别不大时，不推荐使用)
void Transform_color_BW_OSTU(IMAGE im)
{
    // 公式：g = w0 / (1 - w0) * (u0 - u)* (u0 - u) 当g最大时取到阈值T
    int colorMap[256] = { 0 };
    float w0 = 0; // 前景像素数的占比
    unsigned int u0 = 0; // 前景的平局灰度（灰度值*其对应的素数个数）的累加/前景的像素个数
    unsigned int u = 0;  // 图像的平局灰度（灰度值*其对应的素数个数）的累加/总的像素个数
    float g = 0;  // 方差
    unsigned char T = 0;    // 阈值

    // 创建灰度直方图
    for (unsigned int i = 0; i < im.w * im.h; i++)
        colorMap[im.color[i].blue] += 1;
  
    for (int i = 0; i < 256; i++)
         u += colorMap[i] * i;  // u暂时计算累加
    u /= (im.h * im.w);

    // 遍历 0-255 寻找合适的阈值 
    for(unsigned int m = 0 ; m < 256; m++)
    {
        for (int n = m; n < 256; n++)
        {
            w0 += colorMap[n]; // w0暂时计算，保存前景的所有像素个数
            u0 += colorMap[n] * n; // u0暂时计算累加
        }
        u0 /= w0;
        w0 /= (im.h * im.w);
        
        if((w0 / (1 - w0) * (u0 - u)* (u0 - u)) > g)
            g = w0 / (1 - w0) * (u0 - u)* (u0 - u), T = m;
    }
    Transform_color_BW_DIY(im, T);
}


// 二值图(三角法TRIANGLE，适用单峰直方图。当图像的整体颜色差别不大时，不推荐使用)
void Transform_color_BW_TRIANGLE(IMAGE im)
{
    int colorMap[256] = { 0 };
    unsigned char minColor = 0;
    unsigned int minCount = 0;
    unsigned char maxColor = 0;
    unsigned int maxCount = 0;
    int d = 0;  // 最短距离
    unsigned char T = 0; // 阈值

    // 创建灰度直方图
    for (unsigned int i = 0; i < im.w * im.h; i++)
        colorMap[im.color[i].blue] += 1;

    for (int i = 0; i < 256; i++)
    {
        if (im.color[i].blue < minColor)
            minColor = im.color[i].blue, minCount = colorMap[im.color[i].blue];
        if (im.color[i].blue > maxColor)
            maxColor = im.color[i].blue, maxCount = colorMap[im.color[i].blue];
    }

    float k = ((float)maxCount - minCount) / ((float)maxColor - minColor);
    float b = maxCount - k * maxColor;
   
    // 遍历寻找最近距离 
    for (unsigned int n = minColor; n <= maxColor; n++)
        if (abs((int)(-k * n + colorMap[n] - b)) / sqrt((double)(1 + k * k)) > b)
            b = abs((int)(-k * n + colorMap[n] - b)) / sqrt((double)(1 + k * k)), T = n;

    Transform_color_BW_DIY(im, T);
}


// 二值图(自适应阈值法，areaSize=25较合适，当图片线条多且密时，不推荐使用)
IMAGE Transform_color_BW_Adaptive(IMAGE im, int areaSize/*区域的大小*/)
{
    // areaSize为区域的大小，区域越大，效果图的细节越好，areaSize=25较合适
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);
    int* p = (int*)malloc(sizeof(int) * areaSize); // p->position 位置坐标
    int k = (int)(sqrt((double)areaSize)) / 2;  // 重合区域边长的一半

    for (unsigned int  i = 0; i < im.w * im.h; i++)
    {
        // 计算与卷积和对应重合区域的坐标
        int t = 0; // 记录p的下标
        for (int n = k; n >= -k; n--)
            for (int m = -k; m <= k; m++)
            {
                p[t] = ((i % im.w) + m) + (i / im.w + n) * im.w;
                t++;
            }

        // 判断是否越界
        for (int j = 0; j < areaSize; j++)
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;

        unsigned int color = 0;
        for (int j = 0; j < areaSize; j++)
            color += im.color[p[j]].blue;
        color /= areaSize;

        if (im.color[i].blue >= color)
            bgra[i].blue = 255;
        else
            bgra[i].blue = 0;

        bgra[i].green = bgra[i].blue;
        bgra[i].red = bgra[i].blue;
    }

    free(p);
    free(im.color);
    im.color = bgra;
    return im;
}


// 二值图(用二值图表示灰度变化,areaSize=25较合适)
void Transform_color_BW_grayscale(IMAGE im, int areaSize)
{
    // areaSize为区域的大小，区域越大，效果图的细节越好，areaSize=25较合适
    int* p = (int*)malloc(sizeof(int) * areaSize); // p->position 位置坐标
    int k = (int)(sqrt((double)areaSize)) / 2;  // 重合区域边长的一半

    for (unsigned int  i = 0; i < im.w * im.h; i++)
    {
        // 计算与卷积和对应重合区域的坐标
        int t = 0; // 记录p的下标
        for (int n = k; n >= -k; n--)
            for (int m = -k; m <= k; m++)
            {
                p[t] = ((i % im.w) + m) + (i / im.w + n) * im.w;
                t++;
            }

        // 判断是否越界
        for (int j = 0; j < areaSize; j++)
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;

        unsigned int color = 0;
        for (int j = 0; j < areaSize; j++)
            color += im.color[p[j]].blue;
        color /= areaSize;

        if (im.color[i].blue >= color)
            im.color[i].blue = 255;
        else
            im.color[i].blue = 0;

        im.color[i].green = im.color[i].blue;
        im.color[i].red = im.color[i].blue;
    }

    free(p);
}

// 反色
void Transform_color_opposite(IMAGE im)
{
    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        im.color[i].green = 255 - im.color[i].green;
        im.color[i].blue = 255 - im.color[i].blue;
        im.color[i].red = 255 - im.color[i].red;
    }

}


// 直方图均衡化（分步计算，效果更加柔和）
void Transform_color_Histogram_part(IMAGE im)
{
    // 公式：均衡后的颜色值=（最大颜色位255-最小颜色位0）*小于等于该颜色值的像素数量的累加/图片总的像素数

    int Accumulate = 0;             // 保存累加的值
    unsigned char color = 0;        // 保存颜色的值
    int allBlue[256] = { 0 };       // 保存蓝色直方图
    int allGreen[256] = { 0 };      // 保存绿色直方图
    int allRed[256] = { 0 };        // 保存红色直方图

    // 数组下标等于RBG值极大的简化了计算
    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        allBlue[im.color[i].blue] += 1;
        allGreen[im.color[i].green] += 1;
        allRed[im.color[i].red] += 1;
    }

    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        //blue
        for (int j = 0; j <= im.color[i].blue; j++) // 累加计算
            Accumulate += allBlue[j];
        color = (255 - 0) * Accumulate / (im.w * im.h);
        im.color[i].blue = color;
        Accumulate = 0;

        //green
        for (int j = 0; j <= im.color[i].green; j++)// 累加计算
            Accumulate += allGreen[j];
        color = (255 - 0) * Accumulate / (im.w * im.h);
        im.color[i].green = color;
        Accumulate = 0;

        //red
        for (int j = 0; j <= im.color[i].red; j++)// 累加计算
            Accumulate += allRed[j];
        color = (255 - 0) * Accumulate / (im.w * im.h);
        im.color[i].red = color;
        Accumulate = 0;
    }
}



// 直方图均衡化（整体计算，效果更加尖锐）
void Transform_color_Histogram_all(IMAGE im)
{
    int Accumulate = 0;         // 保存累加的值
    unsigned char color = 0;    // 保存颜色的值
    int allColor[256] = { 0 };  // 保存所有颜色直方图

    // 数组下标等于RBG值极大的简化了计算
    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        allColor[im.color[i].blue] += 1;
        allColor[im.color[i].green] += 1;
        allColor[im.color[i].red] += 1;
    }

    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        //blue
        for (int j = 0; j <= im.color[i].blue; j++)// 累加计算
            Accumulate += allColor[j];
        color = (255 - 0) * Accumulate / (im.w * im.h);
        im.color[i].blue = color;
        Accumulate = 0;

        //green
        for (int j = 0; j <= im.color[i].green; j++)// 累加计算
            Accumulate += allColor[j];
        color = (255 - 0) * Accumulate / (im.w * im.h);
        im.color[i].green = color;
        Accumulate = 0;

        //red
        for (int j = 0; j <= im.color[i].red; j++)// 累加计算
            Accumulate += allColor[j];
        color = (255 - 0) * Accumulate / (im.w * im.h);
        im.color[i].red = color;
        Accumulate = 0;
    }
}





// **************************************************************************************************
// 图像的卷积和类卷积操作


// 卷积操作（自定义）
IMAGE Kernels_use_DIY(IMAGE im, double* kernels, int areaSize, double modulus)
{
    // kernels卷积核
    // areaSize区域的大小
    // modulus最后乘的系数

    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);
    int* p = (int*)malloc(sizeof(int) * areaSize); // p->position 位置坐标
    int k = (int)(sqrt((double)areaSize)) / 2;  // 重合区域边长的一半

    for (unsigned int  i = 0; i < im.w * im.h; i++)
    {
        // 计算与卷积和对应重合区域的坐标
        int t = 0; // 记录p的下标
        for(int n = k; n >= -k; n--)
            for (int m = -k; m <= k; m++)
                p[t] = ((i % im.w) + m) + (i / im.w + n) * im.w, t++;
                
        // 判断是否越界
        for (int j = 0; j < areaSize; j++) 
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;

        // 相乘相加
        int blue = 0, green = 0, red = 0;
        for (int j = 0; j < areaSize; j++)
        {
            blue += im.color[p[j]].blue * kernels[j];
            green += im.color[p[j]].green * kernels[j];
            red += im.color[p[j]].red * kernels[j];
        }
        
        bgra[i].blue = Tool_RBG(blue * modulus);
        bgra[i].green = Tool_RBG(green * modulus);
        bgra[i].red = Tool_RBG(red * modulus);
    }

    free(p);
    free(im.color);
    im.color = bgra;
    return im;
}


// **************************************************************************************************
// 滤波 

//中值滤波
IMAGE Wavefiltering_Median(IMAGE im)
{
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);

    for (int unsigned i = 0; i < im.w * im.h; i++)
    {
        // 与卷积和对应重合区域的坐标
        int p[9] =   // p->position 位置坐标
        {
            i + im.w - 1,i + im.w,i + im.w + 1,
            i - 1,i,i + 1,
            i - im.w - 1,i - im.w,i - im.w + 1
        };

        // 判断是否越界
        for (int j = 0; j < 9; j++)
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;

        // 取颜色
        int color[9] =
        {
            im.color[p[0]].blue, im.color[p[1]].blue, im.color[p[2]].blue,
            im.color[p[3]].blue, im.color[p[4]].blue, im.color[p[5]].blue,
            im.color[p[6]].blue, im.color[p[7]].blue, im.color[p[8]].blue
        };

        for (int n = 0; n < 9; n++)  // 对颜色进行排序
            for (int m = n; m < 9; m++)
                if (color[n] > color[m])   // 异或交换不能用等于号
                {
                    color[n] ^= color[m];
                    color[m] ^= color[n];
                    color[n] ^= color[m];
                }

        bgra[i].blue = color[4];
        bgra[i].green = color[4];
        bgra[i].red = color[4];
    }

    free(im.color);
    im.color = bgra;
    return im;
}



//高斯滤波卷积核
double KERNELS_Wave_Gauss[9] =
{
    1, 2, 1,
    2, 4, 2,
    1, 2 ,1
};

//高斯滤波
IMAGE Wavefiltering_Gauss(IMAGE im)
{
    im = Kernels_use_DIY(im, KERNELS_Wave_Gauss, 9, 1.0 / 16);
    return im;
}


// 低通滤波卷积核 LP1
double KERNELS_Wave_LowPass_LP1[9] =
{
    1 / 9.0, 1 / 9.0, 1 / 9.0,
    1 / 9.0, 1 / 9.0, 1 / 9.0,
    1 / 9.0, 1 / 9.0, 1 / 9.0
};

// 低通滤波卷积核 LP2
double KERNELS_Wave_LowPass_LP2[9] =
{
    1 / 10.0, 1 / 10.0, 1 / 10.0,
    1 / 10.0, 1 / 5.0, 1 / 10.0,
    1 / 10.0, 1 / 10.0, 1 / 10.0
};

// 低通滤波卷积核 LP3
double KERNELS_Wave_LowPass_LP3[9] =
{
    1 / 16.0, 1 / 8.0, 1 / 16.0,
    1 / 8.0, 1 / 4.0, 1 / 8.0,
    1 / 16.0, 1 / 8.0, 1 / 16.0
};


// 低通滤波
IMAGE Wavefiltering_LowPass(IMAGE im, double* kernels)
{
    im = Kernels_use_DIY(im, kernels, 9, 1);
    return im;
}



// 高通滤波卷积核 HP1
double KERNELS_Wave_HighPass_HP1[9] =
{
    -1, -1, -1,
    -1, 9, -1,
    -1, -1 ,-1
};

// 高通滤波卷积核 HP2
double KERNELS_Wave_HighPass_HP2[9] =
{
    0, -1, 0,
   -1, 5, -1,
    0, -1 ,0
};

// 高通滤波卷积核 HP3
double KERNELS_Wave_HighPass_HP3[9] =
{
    1, -2, 1,
   -2, 5, -2,
    1, -2 ,1
};

// 高通滤波
IMAGE Wavefiltering_HighPass(IMAGE im, double* kernels)
{
    im = Kernels_use_DIY(im, kernels, 9, 1);
    return im;
}


// 均值滤波卷积核
double KERNELS_Wave_Average[25] =
{
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1
};

// 均值滤波
IMAGE Wavefiltering_Average(IMAGE im)
{
    im = Kernels_use_DIY(im, KERNELS_Wave_Average, 25, 1.0 / 25);
    return im;
}



// **************************************************************************************************
// 边缘检测

// 差分垂直边缘检测卷积核
double KERNELS_Edge_difference_vertical[9] =
{
    0, 0, 0,
   -1, 1, 0,
    0, 0, 0
};

// 差分水平边缘检测卷积核
double KERNELS_Edge_difference_horizontal[9] =
{
    0,-1, 0,
    0, 1, 0,
    0, 0, 0
};

// 差分垂直和水平边缘检测卷积核
double KERNELS_Edge_difference_VH[9] =
{
   -1, 0, 0,
    0, 1, 0,
    0, 0, 0
};


// 差分边缘检测
IMAGE Edge_detection_difference(IMAGE im, double* kernels)
{
    im = Kernels_use_DIY(im, kernels, 9, 1);
    return im;
}


// Sobel X边缘检测卷积核
double KERNELS_Edge_Sobel_X[9] =
{
    -1, 0, 1,
   - 2, 0, 2,
    -1, 0, 1
};

// Sobel Y边缘检测卷积核
double KERNELS_Edge_Sobel_Y[9] =
{
   -1, -2, -1,
    0, 0, 0,
    1, 2, 1
};

// Sobel边缘检测
IMAGE Edge_detection_Sobel(IMAGE im, double* kernels1, double* kernels2)
{
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);

    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        // 与卷积和对应重合区域的坐标
        int p[9] =   // p->position 位置坐标
        {
            i + im.w - 1,i + im.w,i + im.w + 1,
            i - 1,i,i + 1,
            i - im.w - 1,i - im.w,i - im.w + 1
        };

        for (int j = 0; j < 9; j++) // 判断是否越界
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;


        unsigned char color1 = Tool_RBG(im.color[p[0]].blue * kernels1[0] + im.color[p[1]].blue * kernels1[1] + im.color[p[2]].blue * kernels1[2] + im.color[p[3]].blue * kernels1[3] + im.color[p[4]].blue * kernels1[4] + im.color[p[5]].blue * kernels1[5] + im.color[p[6]].blue * kernels1[6] + im.color[p[7]].blue * kernels1[7] + im.color[p[8]].blue * kernels1[8]);

        if (kernels2 != NULL)
        {
            unsigned char  color2 = Tool_RBG(im.color[p[0]].blue * kernels2[0] + im.color[p[1]].blue * kernels2[1] + im.color[p[2]].blue * kernels2[2] + im.color[p[3]].blue * kernels2[3] + im.color[p[4]].blue * kernels2[4] + im.color[p[5]].blue * kernels2[5] + im.color[p[6]].blue * kernels2[6] + im.color[p[7]].blue * kernels2[7] + im.color[p[8]].blue * kernels2[8]);
            color1 = Tool_RBG(sqrt(color1 * color1 + color2 * color2));
        }

        bgra[i].blue = color1;
        bgra[i].green = color1;
        bgra[i].red = color1;
    }

    free(im.color);
    im.color = bgra;
    return im;
}


// Laplace边缘检测卷积核 LAP1
double KERNELS_Edge_Laplace_LAP1[9] =
{
    0, 1, 0,
    1, -4, 1,
    0, 1, 0
};

// Laplace边缘检测卷积核 LAP2
double KERNELS_Edge_Laplace_LAP2[9] =
{
   -1, -1, -1,
   -1, 8, -1,
   -1, -1, -1
};

// Laplace边缘检测卷积核 LAP3
double KERNELS_Edge_Laplace_LAP3[9] =
{
   -1, -1, -1,
   -1, 9, -1,
   -1, -1, -1
};

// Laplace边缘检测卷积核 LAP4
double KERNELS_Edge_Laplace_LAP4[9] =
{
    1, -2, 1,
   -2, 8, -2,
    1, -2, 1
};


// Laplace边缘检测 
IMAGE Edge_detection_Laplace(IMAGE im, double* kernels)
{
    im = Kernels_use_DIY(im, kernels, 9, 1);
    return im;
}



// **************************************************************************************************
// 形态学图像处理


// 腐蚀卷积核
double KERNELS_Morphology_Erosion_cross[9] =
{
    0, 1, 0,
    1, 1, 1,
    0, 1, 0
};

// 腐蚀
IMAGE Morphology_Erosion(IMAGE im, double* kernels)
{
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);

    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        // 与卷积和对应重合区域的坐标
        int p[9] =   // p->position 位置坐标
        {
            i + im.w - 1,i + im.w,i + im.w + 1,
            i - 1,i,i + 1,
            i - im.w - 1,i - im.w,i - im.w + 1
        };

        for (int j = 0; j < 9; j++) // 判断是否越界
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;

        // 判断是否腐蚀（式子很长，但简单）
        if ((im.color[i].blue == 255) && (im.color[p[0]].blue * kernels[0] + im.color[p[1]].blue * kernels[1] + im.color[p[2]].blue * kernels[2] + im.color[p[3]].blue * kernels[3] + im.color[p[5]].blue * kernels[5] + im.color[p[6]].blue * kernels[6] + im.color[p[7]].blue * kernels[7] + im.color[p[8]].blue * kernels[8]) < 255 * (kernels[0] + kernels[1] + kernels[2] + kernels[3] + kernels[5] + kernels[6] + kernels[7] + kernels[8]))
        {
            bgra[i].blue = 0;
            bgra[i].green = 0;
            bgra[i].red = 0;
        }
        else
        {
            bgra[i].blue = im.color[i].blue;
            bgra[i].green = im.color[i].green;
            bgra[i].red = im.color[i].red;
        }
    }

    free(im.color);
    im.color = bgra;
    return im;
}



// 膨胀卷积核
double KERNELS_Morphology_Dilation_cross[9] =
{
    0, 1, 0,
    1, 1, 1,
    0, 1, 0
};

// 膨胀
IMAGE Morphology_Dilation(IMAGE im, double* kernels)
{
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * im.w * im.h);

    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        // 与卷积和对应重合区域的坐标
        int p[9] =   // p->position 位置坐标
        {
            i + im.w - 1,i + im.w,i + im.w + 1,
            i - 1,i,i + 1,
            i - im.w - 1,i - im.w,i - im.w + 1
        };

        for (int j = 0; j < 9; j++) // 判断是否越界
            if (p[j] < 0 || p[j] >= im.w * im.h)
                p[j] = i;

        // 判断是否膨胀（式子很长，但简单）
        if ((im.color[i].blue == 0) && (im.color[p[0]].blue * kernels[0] + im.color[p[1]].blue * kernels[1] + im.color[p[2]].blue * kernels[2] + im.color[p[3]].blue * kernels[3] + im.color[p[5]].blue * kernels[5] + im.color[p[6]].blue * kernels[6] + im.color[p[7]].blue * kernels[7] + im.color[p[8]].blue * kernels[8]) >= 255)
        {
            bgra[i].blue = 255;
            bgra[i].green = 255;
            bgra[i].red = 255;
        }
        else
        {
            bgra[i].blue = im.color[i].blue;
            bgra[i].green = im.color[i].green;
            bgra[i].red = im.color[i].red;
        }
    }

    free(im.color);
    im.color = bgra;
    return im;
}



// **************************************************************************************************
// 图像的池化操作


//池化
IMAGE Pooling(IMAGE im, int lenght)
{
    // lenght池化区域的边长
    unsigned int width = im.w / lenght;
    unsigned int hight = im.h / lenght;
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * width * hight);
    int* p = (int*)malloc(sizeof(int) * lenght * lenght); // p->position 位置坐标
    unsigned char maxColor = 0;  // 保存区域内的最大颜色值
    int k = 0; // 记录实际循环的次数，作为新图的坐标

    for (unsigned int i = 0; i < im.w * im.h; i += lenght)
    {
        // 计算与卷积和对应重合区域的坐标
        int t = 0; // 记录p的下标
        for (int n = 0; n < lenght; n++)
            for (int m = 0; m < lenght; m++)
                p[t] = ((i % im.w) + m) + (i / im.w + n) * im.w, t++;

        if (p[lenght * lenght - 1] >= im.w * im.h) // 判断上边界
            break;
        else if (i / im.w != 0 && (i / im.w) % lenght != 0) // 判断到了中间行
        {
            i += (lenght - 1) * im.w;
            continue;
        }
        else if ((p[lenght * lenght - 1] / im.w) - (p[0] / im.w) + 1 != lenght)  // 判断右边界
        {
            i = i / im.w * im.w + im.w * lenght - lenght;
            continue;
        }
        else
        {       
            maxColor = im.color[p[0]].blue;     // 计算最大颜色值
            for (int j = 0; j < lenght * lenght; j++)
                if (im.color[p[j]].blue > maxColor)
                    maxColor = im.color[p[j]].blue;

            bgra[k].blue = maxColor, bgra[k].green = maxColor, bgra[k].red = maxColor, k++;
        }
    }

    free(p);
    free(im.color);
    im.color = bgra;
    im.w = width;
    im.h = hight;
    return im;
}



// **************************************************************************************************
// VJ人脸检测


// **************************************************************************************************
// 积分图的处理


// 积分图结构体
typedef struct tagIGIMAGE
{
   unsigned int w;
   unsigned int h;
    int* date;
}IGIMAGE, * PIGIMAGE;


// 获得积分图（在此之前要保证图片是“白底黑字”）
IGIMAGE IntegralImage_get(IMAGE im)
{
    IGIMAGE IGmap;
    int* array = (int*)malloc(sizeof(int) * im.w * im.h);

    int k = 0; // 用于统计每一行的像素个数
    for (unsigned int i = 0; i < im.w * im.h; i++)
    {
        if (i % im.w == 0) // 判断左边界
            k = 0;

        if (im.color[i].blue == 0)// 判断是否有像素
            k++;

        if (i / im.w == 0)  // 统计第一行的数据，这是基础
            array[i] = k;
        else
            array[i] = array[i - im.w] + k;
    }

    IGmap.date = array;
    IGmap.w = im.w;
    IGmap.h = im.h;
    return IGmap;
}


// 计算积分区域像素个数
int IntegralImage_count(IGIMAGE IGmap, int rightTop, int leftBottom)
{
    int a1, a2, a3, a4;
    a1 = leftBottom;
    a2 = (rightTop % IGmap.w) + (leftBottom / IGmap.w) * IGmap.w;
    a3 = (leftBottom % IGmap.w) + (rightTop / IGmap.w) * IGmap.w;
    a4 = rightTop;

    //  判断是否越界
    if (a1 < 0)
        a1 = 0;
    if (a2 < 0)
        a2 = 0;
    if (a3 < 0)
        a3 = 0;
    if (a3 > IGmap.w * IGmap.h - 1)
        a3 = a4;

    // 计算区域中的像素数
    return IGmap.date[a4] - IGmap.date[a3] - IGmap.date[a2] + IGmap.date[a1];
}


// 释放积分图结构体
void IntegralImage_free(IGIMAGE IGimage)
{
    free(IGimage.date);
}


// **************************************************************************************************
// 单分支决策树分类器

// 单分支决策树分类器
double Classifier_decisionStump(IGIMAGE IGmap, int rightTop, int leftBottom)
{   
    // 计算所判定区域的宽和高
    int areaW = (rightTop % IGmap.w) - (leftBottom % IGmap.w);
    int areaH = (rightTop / IGmap.w) - (leftBottom / IGmap.w);

    int x0 = leftBottom % IGmap.w;
    int y0 = leftBottom / IGmap.w;
    int x1 = rightTop % IGmap.w;
    int y1 = rightTop / IGmap.w;

    // 计算25个区域的像素个数
    int w_all = IntegralImage_count(IGmap, rightTop, leftBottom);

    int w_1 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + y1 * IGmap.w, x0 + (y1 - areaH / 5) * IGmap.w);
    int w_2 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + y1 * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH / 5) * IGmap.w);
    int w_3 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + y1 * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH / 5) * IGmap.w);
    int w_4 = IntegralImage_count(IGmap, (x1 - areaW / 5) + y1 * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH / 5) * IGmap.w);
    int w_5 = IntegralImage_count(IGmap, x1 + y1 * IGmap.w, (x1 - areaW * 1 / 5) + (y1 - areaH / 5) * IGmap.w);

    int w_6 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH / 5) * IGmap.w, x0 + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_7 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH / 5) * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_8 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH / 5) * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_9 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH / 5) * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_10 = IntegralImage_count(IGmap, x1 + (y1 - areaH / 5) * IGmap.w, (x1 - areaW / 5) + (y1 - areaH * 2 / 5) * IGmap.w);

    int w_11 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH * 2 / 5) * IGmap.w, x0 + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_12 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_13 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_14 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_15 = IntegralImage_count(IGmap, x1 + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW / 5) + (y1 - areaH * 3 / 5) * IGmap.w);

    int w_16 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH * 3 / 5) * IGmap.w, x0 + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_17 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_18 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_19 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_20 = IntegralImage_count(IGmap, x1 + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW / 5) + (y1 - areaH * 4 / 5) * IGmap.w);

    int w_21 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH * 4 / 5) * IGmap.w, x0 + y0 * IGmap.w);
    int w_22 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW * 4 / 5) + y0 * IGmap.w);
    int w_23 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW * 3 / 5) + y0 * IGmap.w);
    int w_24 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW * 2 / 5) + y0 * IGmap.w);
    int w_25 = IntegralImage_count(IGmap, x1 + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW / 5) + y0 * IGmap.w);

    // 判断是否为人脸
    if ((double)w_all / (areaW * areaH) < 0.19)
        return 1;
    if ((double)(w_1 + w_2 + w_6 + w_7) / (w_3 + w_8) < 2.6 || (double)(w_4 + w_5 + w_9 + w_10) / (w_3 + w_8) < 2.6)
        return 1;
    if ((double)(w_13 + w_18) / (w_11  + w_16 ) < 1 || (double)(w_13 + w_18) / ( w_15 + w_20) < 1)
        return 1;
    if ((double)(w_1 + w_2 + w_6 + w_7) / (w_11 + w_12 + w_16 + w_17) < 1.3 || (double)(w_4 + w_5 + w_9 + w_10) / (w_14 + w_15 + w_19 + w_20) < 1.3)
        return 1;
    if ((double)(w_1 + w_2 + w_3 + w_4 + w_5 + w_6 + w_7 + w_8 + w_9 + w_10) / (w_16 + w_17 + w_18 + w_19 + w_20 + w_21 + w_22 + w_23 + w_24 + w_25) > 2)
        return 1;
    if ((double)(w_1 + w_2 + w_6 + w_7 + w_4 + w_5 + w_9 + w_10 + w_13 + w_17 + w_18 + w_19 + w_23) / w_all < 0.6)
        return 1;

    double PCT_1 = (double)min(w_1 + w_2 + w_6 + w_7 + w_11 + w_12 + w_16 + w_17 + w_21 + w_22, w_4 + w_5 + w_9 + w_10 + w_14 + w_15 + w_19 + w_20 + w_24 + w_25) / max(w_1 + w_2 + w_6 + w_7 + w_11 + w_12 + w_16 + w_17 + w_21 + w_22, w_4 + w_5 + w_9 + w_10 + w_14 + w_15 + w_19 + w_20 + w_24 + w_25);
    PCT_1 = exp(-3.125 * (PCT_1 - 1) * (PCT_1 - 1)) * 100;

    double PCT_2 = (double)min(w_1 + w_2 + w_6 + w_7, w_4 + w_5 + w_9 + w_10) / max(w_1 + w_2 + w_6 + w_7, w_4 + w_5 + w_9 + w_10);
    PCT_2 = exp(-3.125 * (PCT_1 - 1) * (PCT_1 - 1)) * 100;

    double PCT_3 = (double)min(w_16 + w_21, w_20 + w_25) / max(w_16 + w_21, w_20 + w_25);
    PCT_3 = exp(-3.125 * (PCT_3 - 1) * (PCT_3 - 1)) * 100;

    // 计算总的概率
    double PCT_all = (PCT_1 + PCT_2 + PCT_3) / 3;

    if (PCT_all > 60)
        return PCT_all;
}

// 单分支决策树分类器（训练用）
int Classifier_decisionStump_train(IMAGE im, IGIMAGE IGmap, int rightTop, int leftBottom)
{
    // 计算所判定区域的宽和高
    int areaW = (rightTop % IGmap.w) - (leftBottom % IGmap.w);
    int areaH = (rightTop / IGmap.w) - (leftBottom / IGmap.w);

    int x0 = leftBottom % IGmap.w;
    int y0 = leftBottom / IGmap.w;
    int x1 = rightTop % IGmap.w;
    int y1 = rightTop / IGmap.w;


    int w_all = IntegralImage_count(IGmap, rightTop, leftBottom);

    int w_1 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + y1 * IGmap.w, x0 + (y1 - areaH / 5) * IGmap.w);
    int w_2 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + y1 * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH / 5) * IGmap.w);
    int w_3 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + y1 * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH / 5) * IGmap.w);
    int w_4 = IntegralImage_count(IGmap, (x1 - areaW / 5) + y1 * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH / 5) * IGmap.w);
    int w_5 = IntegralImage_count(IGmap, x1 + y1 * IGmap.w, (x1 - areaW * 1 / 5) + (y1 - areaH / 5) * IGmap.w);

    int w_6 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH / 5) * IGmap.w, x0 + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_7 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH / 5) * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_8 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH / 5) * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_9 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH / 5) * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH * 2 / 5) * IGmap.w);
    int w_10 = IntegralImage_count(IGmap, x1 + (y1 - areaH / 5) * IGmap.w, (x1 - areaW / 5) + (y1 - areaH * 2 / 5) * IGmap.w);

    int w_11 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH * 2 / 5) * IGmap.w, x0 + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_12 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_13 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_14 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH * 3 / 5) * IGmap.w);
    int w_15 = IntegralImage_count(IGmap, x1 + (y1 - areaH * 2 / 5) * IGmap.w, (x1 - areaW / 5) + (y1 - areaH * 3 / 5) * IGmap.w);

    int w_16 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH * 3 / 5) * IGmap.w, x0 + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_17 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW * 4 / 5) + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_18 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW * 3 / 5) + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_19 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW * 2 / 5) + (y1 - areaH * 4 / 5) * IGmap.w);
    int w_20 = IntegralImage_count(IGmap, x1 + (y1 - areaH * 3 / 5) * IGmap.w, (x1 - areaW / 5) + (y1 - areaH * 4 / 5) * IGmap.w);

    int w_21 = IntegralImage_count(IGmap, (x1 - areaW * 4 / 5) + (y1 - areaH * 4 / 5) * IGmap.w, x0 + y0 * IGmap.w);
    int w_22 = IntegralImage_count(IGmap, (x1 - areaW * 3 / 5) + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW * 4 / 5) + y0 * IGmap.w);
    int w_23 = IntegralImage_count(IGmap, (x1 - areaW * 2 / 5) + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW * 3 / 5) + y0 * IGmap.w);
    int w_24 = IntegralImage_count(IGmap, (x1 - areaW / 5) + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW * 2 / 5) + y0 * IGmap.w);
    int w_25 = IntegralImage_count(IGmap, x1 + (y1 - areaH * 4 / 5) * IGmap.w, (x1 - areaW / 5) + y0 * IGmap.w);



    if ((double)w_all / (areaW * areaH) < 0.19)
        return 1;
    if ((double)(w_1 + w_2 + w_6 + w_7) / (w_3 + w_8) < 2.6 || (double)(w_4 + w_5 + w_9 + w_10) / (w_3 + w_8) < 2.6)
        return 2;
    if ((double)(w_13 + w_18) / (w_11 + w_16) < 1 || (double)(w_13 + w_18) / (w_15 + w_20) < 1)
        return 3;
    if ((double)(w_1 + w_2 + w_6 + w_7) / (w_11 + w_12 + w_16 + w_17) < 1.22 || (double)(w_4 + w_5 + w_9 + w_10) / (w_14 + w_15 + w_19 + w_20) < 1.22)
        return 4;
    if ((double)(w_1 + w_2 + w_3 + w_4 + w_5 + w_6 + w_7 + w_8 + w_9 + w_10) / (w_16 + w_17 + w_18 + w_19 + w_20 + w_21 + w_22 + w_23 + w_24 + w_25) > 2)
        return 5;
    if ((double)(w_1 + w_2 + w_6 + w_7 + w_4 + w_5 + w_9 + w_10 + w_13 + w_17 + w_18 + w_19 + w_23) / w_all < 0.6)
        return 6;

    double PCT_1 = (double)min(w_1 + w_2 + w_6 + w_7 + w_11 + w_12 + w_16 + w_17 + w_21 + w_22, w_4 + w_5 + w_9 + w_10 + w_14 + w_15 + w_19 + w_20 + w_24 + w_25) / max(w_1 + w_2 + w_6 + w_7 + w_11 + w_12 + w_16 + w_17 + w_21 + w_22, w_4 + w_5 + w_9 + w_10 + w_14 + w_15 + w_19 + w_20 + w_24 + w_25);
    PCT_1 = exp(-3.125 * (PCT_1 - 1) * (PCT_1 - 1)) * 100;

    double PCT_2 = (double)min(w_1 + w_2 + w_6 + w_7, w_4 + w_5 + w_9 + w_10) / max(w_1 + w_2 + w_6 + w_7, w_4 + w_5 + w_9 + w_10);
    PCT_2 = exp(-3.125 * (PCT_1 - 1) * (PCT_1 - 1)) * 100;

    double PCT_3 = (double)min(w_16 + w_21, w_20 + w_25) / max(w_16 + w_21, w_20 + w_25);
    PCT_3 = exp(-3.125 * (PCT_3 - 1) * (PCT_3 - 1)) * 100;

    // 计算总的概率
    double PCT_all = (PCT_1 + PCT_2 + PCT_3) / 3;



    if (PCT_all > 60)
    {
        printf("%lf", PCT_all);
        printf(" %lf\n", (double)(w_1 + w_2 + w_6 + w_7 + w_4 + w_5 + w_9 + w_10 + w_13 + w_17 + w_18 + w_19 + w_23) / w_all);

        //画出人脸框
        for (unsigned int i = leftBottom / im.w; i <= rightTop / im.w; i++) {
            for (unsigned int j = leftBottom % im.w; j <= rightTop % im.w; j++) {
                if (i == leftBottom / im.w || i == rightTop / im.w || j == leftBottom % im.w || j == rightTop % im.w) {
                    im.color[j + i * im.w].blue = 0;
                    im.color[j + i * im.w].green = 0;
                    im.color[j + i * im.w].red = 200;
                }
            }
        }

        return 9;

    }
    else
        return 5;

}
 

// **************************************************************************************************
// 多尺度滑动窗口计术


// 人脸数据结构体
typedef struct tagFACEDATE
{
    int rightTop;
    int leftBottom;
    double confidence;
}FACEDATE;


//滑动窗口区域（训练用）
FACEDATE MoveWindowArea(IMAGE im, IGIMAGE IGmap)
{
    FACEDATE maxFaceDate = { 0 };       // 保存概率最大的人脸区域
    double confidence = 0;              // 置信度
    int minSide = min(im.w, im.h) / 3;  // 最小区域
    int daltaSide = 5;                  // 区域每次的增加量
    int k = 0;                          // faceDate结构体数组的下标
    
    // 窗口区域的取值范围
    for (int i = 0; i <= (min(im.w, im.h) - minSide) / daltaSide - 1; i++)
    {
        int rightTop = (minSide + i * daltaSide) * (im.w + 1);
        int leftBottom = 0;

        while (rightTop != im.w * im.h - 1)
        {
            if ((rightTop + 1) % im.w == 0)
            {
                rightTop += minSide + i * daltaSide;
                leftBottom += minSide + i * daltaSide;
            }
            else
            {
                rightTop += 1;
                leftBottom += 1;
            }

            if ((confidence = Classifier_decisionStump(IGmap, rightTop, leftBottom)) > 1 && confidence > maxFaceDate.confidence)
            {
                maxFaceDate.confidence = confidence;
                maxFaceDate.rightTop = rightTop;
                maxFaceDate.leftBottom = leftBottom;
            }
            
        }
    }

    return maxFaceDate;

    ////画出人脸框
    //for (unsigned int i = maxFaceDate.leftBottom / im.w; i <= maxFaceDate.rightTop / im.w; i++) {
    //    for (unsigned int j = maxFaceDate.leftBottom % im.w; j <= maxFaceDate.rightTop % im.w; j++) {
    //        if (i == maxFaceDate.leftBottom / im.w || i == maxFaceDate.rightTop / im.w || j == maxFaceDate.leftBottom % im.w || j == maxFaceDate.rightTop % im.w) {
    //            im.color[j + i * im.w].blue = 0;
    //            im.color[j + i * im.w].green = 0;
    //            im.color[j + i * im.w].red = 200;
    //        }
    //    }
    //}
}

//滑动窗口区域（训练用）
void MoveWindowArea_train(IMAGE im, IGIMAGE IGmap)
{
    int minSide = min(im.w, im.h) / 3;  //最小区域
    int daltaSide = 5;   //区域每次的增加量

    int ppp[10] = {0};

    // 窗口区域的取值范围
    for (int i = 0; i <= (min(im.w, im.h) - minSide) / daltaSide - 1; i++)
    {
        int rightTop = (minSide + i * daltaSide) * (im.w + 1);
        int leftBottom = 0;

        printf("%d\n", rightTop);

        while (rightTop != im.w * im.h - 1)
        {
            if ((rightTop + 1) % im.w == 0)
            {
                rightTop += minSide + i * daltaSide;
                leftBottom += minSide + i * daltaSide;
            }
            else
            {
                rightTop += 1;
                leftBottom += 1;
            }
            ppp[Classifier_decisionStump_train(im, IGmap, rightTop, leftBottom)] += 1;    
        }
    }

    printf("\n1:%d 2:%d 3:%d 4:%d 5:%d 6:%d\n", ppp[1], ppp[2], ppp[3], ppp[4], ppp[5], ppp[6]);
}



// **************************************************************************************************
// 画出人框

// 画出人框
void Image_draw(IMAGE im ,FACEDATE faceDate)
{
    //画出人脸框
    for (unsigned int i = faceDate.leftBottom / im.w; i <= faceDate.rightTop / im.w; i++) {
        for (unsigned int j = faceDate.leftBottom % im.w; j <= faceDate.rightTop % im.w; j++) {
            if (i == faceDate.leftBottom / im.w || i == faceDate.rightTop / im.w || j == faceDate.leftBottom % im.w || j == faceDate.rightTop % im.w) {
                im.color[j + i * im.w].blue = 0;
                im.color[j + i * im.w].green = 0;
                im.color[j + i * im.w].red = 200;
            }
        }
    }
}





}

#endif