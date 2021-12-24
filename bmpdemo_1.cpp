#include "bmp.h"
#include <iostream>
#include <string.h>
#include <cmath>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

using namespace std;

#define IMG_FILE_INPUT_PATH "../output/test.jpg"
#define IMG_FILE_OUT_PUT_PATH "../output/test.bmp"
#define IMG_FILE_OUT_PUT_PATH_2 "../output/test2.bmp"

/**
 * @brief 24bpp scale
 * 
 * @return int 
 */
int main()
{

    cv::Mat src = cv::imread("../input/demo.jpg");
    cv::cvtColor(src, src, CV_BGR2GRAY);
    int width = src.cols;
    int height = src.rows;
    LOG("w:%d h%d chanel:%d", width, height, src.channels());

    BMPFileHeader bmpfileHeader;
    bmpfileHeader.bfType = ('B' | 'M' << 8);
    bmpfileHeader.bfReserved1 = 0;
    bmpfileHeader.bfReserved2 = 0;

    BMPInfoHeader bmpInfoHeader;

    //初始化顔色表
    BMPRgbQuad palette[2];
    palette[0].red = 0;
    palette[0].green = 0;
    palette[0].blue = 0;
    palette[0].alpha = 0;

    palette[1].red = 255;
    palette[1].green = 255;
    palette[1].blue = 255;
    palette[1].alpha = 255;

    //文件头配置
    bmpfileHeader.bfOffBits = sizeof(bmpfileHeader) + sizeof(bmpInfoHeader);
    unsigned int bmpLineWidth = alloc_line_width(24, width);
    bmpfileHeader.bfSize = bmpfileHeader.bfOffBits + bmpLineWidth * height;

    //图片信息头配置
    bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitcount = 24;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.biSizeImage = bmpLineWidth * height;
    bmpInfoHeader.biXpelsPermeter = 0;
    bmpInfoHeader.biYPelsPermeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    unsigned char *dst;
    FILE *fout;

    dst = (unsigned char *)malloc(bmpLineWidth * height);

    memset(dst, 0, bmpLineWidth * height);

    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            int gray = src.data[1 * (i * src.cols + j)];
            gray = gray > 128 ? 255 : 0;
            int ix = j * 3;
            dst[((height - 1) - i) * bmpLineWidth + ix] = gray;
            dst[((height - 1) - i) * bmpLineWidth + ix + 1] = gray;
            dst[((height - 1) - i) * bmpLineWidth + ix + 2] = gray;
        }
    }
    int newHeight = height * 2;
    int newWidth = width * 2;

    unsigned int lineByte = alloc_line_width(24, newWidth);
    unsigned char *dsts = (unsigned char *)malloc(lineByte * newHeight);
    memset(dsts, 0, lineByte * newHeight);
    for (int i = 0; i < newHeight; i++)
    {

        int x = round(i * 1.0f * height / newHeight);
        for (int j = 0; j < lineByte; j++)
        {
            int y = round(j * 1.0f * bmpLineWidth / lineByte);
            int b = dst[x * bmpLineWidth + y];
            int g = dst[x * bmpLineWidth + y + 1];
            int r = dst[x * bmpLineWidth + y + 2];

            int gray = 0.3 * r + 0.59 * g + 0.11 * b;
            gray = gray > 128 ? 255 : 0;

            dsts[i * lineByte + j] = gray;
            dsts[i * lineByte + j + 1] = gray;
            dsts[i * lineByte + j + 2] = gray;
            j += 2;
        }
    }

    bmpfileHeader.bfSize = lineByte * newHeight + bmpfileHeader.bfOffBits;
    bmpInfoHeader.biHeight = newHeight;
    bmpInfoHeader.biWidth = newWidth;
    bmpInfoHeader.biSizeImage = lineByte * newHeight;

    fout = fopen("../output/bmpdemo_1.bmp", "wb");
    fwrite(&bmpfileHeader, sizeof(bmpfileHeader), 1, fout);
    fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fout);
    // fwrite(&palette, sizeof(palette), 1, fout);
    fwrite(dsts, lineByte * newHeight, 1, fout);
    fclose(fout);
    printf("ok!\n");
    return 0;
}