#include "bmp.h"
#include <iostream>
#include <string.h>
#include <cmath>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "bmputil.h"

/**
 * @brief 1bpp scale
 *
 * @return int
 */
int main()
{

    cv::Mat src = cv::imread("../input/demo.jpg");
    int width = src.cols;
    int height = src.rows;
    LOG("w:%d h%d chanel:%d", width, height, src.channels());
    cv::cvtColor(src, src, CV_BGR2GRAY);

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
    palette[1].alpha = 0;

    //文件头配置
    bmpfileHeader.bfOffBits = sizeof(bmpfileHeader) + sizeof(bmpInfoHeader) + sizeof(palette);
    unsigned int bmpLineWidth = alloc_line_width(1, width);
    bmpfileHeader.bfSize = bmpfileHeader.bfOffBits + bmpLineWidth * height;

    //图片信息头配置
    bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitcount = 1;
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

    //做原始素材
    for (int i = height - 1; i >= 0; i--)
    {
        int ix = 0;
        for (int j = 0; j < width; j++)
        {
            int gray = src.data[1 * (i * width + j)];
            gray = gray > 128 ? 1 : 0;
            unsigned char piexl = gray << (7 - j % 8);
            ix = j / 8;
            dst[((height - 1) - i) * bmpLineWidth + ix] |= piexl;
        }
    }
    BMPFile bmp;
    bmp.data = dst;
    bmp.fileHeader = bmpfileHeader;
    bmp.infoHeader = bmpInfoHeader;
    bmp.palette = palette;
    scaleBy1bpp2(&bmp, width * 2, height * 2);

    fout = fopen("../output/bmpdemo_2.bmp", "wb");
    fwrite(&bmp.fileHeader, sizeof(bmp.fileHeader), 1, fout);
    fwrite(&bmp.infoHeader, sizeof(bmp.infoHeader), 1, fout);
    fwrite(bmp.palette, sizeof(bmp.palette), 1, fout);
    fwrite(bmp.data, bmp.infoHeader.biSizeImage, 1, fout);
    fclose(fout);

    return 0;
}