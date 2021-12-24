#include "bmputil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

/**
 * @brief 1bpp scale
 *
 * @param bmpfile
 * @param newWidth
 * @param newHeight
 */
void scaleBy1bpp1(BMPFile *bmpfile, int newWidth, int newHeight)
{
    BMPFileHeader bmpfileHeader = bmpfile->fileHeader;
    BMPInfoHeader bmpInfoHeader = bmpfile->infoHeader;
    unsigned char *data = bmpfile->data;
    BMPRgbQuad *palette = bmpfile->palette;
    int width = bmpInfoHeader.biWidth;
    int height = bmpInfoHeader.biHeight;
    int bmpLineWidth = alloc_line_width(1, width);
    int lineBytes = alloc_line_width(1, newWidth);

    unsigned char *dst;
    dst = (unsigned char *)malloc(lineBytes * newHeight);
    memset(dst, 0, lineBytes * newHeight);

    for (int i = 0; i < newHeight; i++)
    {
        int x = round(i * 1.0f * width / newWidth);
        for (int j = 0; j < lineBytes * 8; j++)
        {
            int rj = j / 8;
            int rm = j % 8;
            int y = round(rj * 1.0f * height / newHeight);
            unsigned char piexl = data[x * bmpLineWidth + y];
            piexl = (piexl >> (7 - rm)) & 0x1;
            dst[i * lineBytes + rj] |= piexl << rm;
        }
    }

    bmpfileHeader.bfSize = lineBytes * newHeight + bmpfileHeader.bfOffBits;
    bmpInfoHeader.biHeight = newHeight;
    bmpInfoHeader.biWidth = newWidth;
    bmpInfoHeader.biSizeImage = lineBytes * newHeight;
    bmpInfoHeader.biBitcount = 1;

    bmpfile->fileHeader = bmpfileHeader;
    bmpfile->data = dst;
    bmpfile->infoHeader = bmpInfoHeader;
    bmpfile->palette = palette;
}

void scaleBy1bpp2(BMPFile *bmpFile, int newWidth, int newHeight)
{
    BMPFileHeader bmpFileHeader = bmpFile->fileHeader;
    BMPInfoHeader bmpInfoHeader = bmpFile->infoHeader;
    unsigned char *data = bmpFile->data;
    BMPRgbQuad *palette = bmpFile->palette;
    int width = bmpInfoHeader.biWidth;
    int height = bmpInfoHeader.biHeight;
    int bmpLineWidth = alloc_line_width(1, width);

    unsigned int scaleLineByte = alloc_line_width(24, width);
    unsigned char *scaleDst;
    scaleDst = (unsigned char *)malloc(scaleLineByte * height);
    memset(scaleDst, 0, scaleLineByte * height);

    // 1bpp to 24bpp
    int px;
    int offsetWidth = width / 8 - (bmpLineWidth - (width / 8));

    for (int i = 0; i < height; i++)
    {
        px = 0;
        for (int j = 0; j < offsetWidth; j++)
        {
            for (int k = 7; k >= 0; k--)
            {
                unsigned char piexl = (data[i * bmpLineWidth + j] >> k) & 0x1;
                piexl = piexl == 1 ? 255 : 0;
                scaleDst[i * scaleLineByte + px] = piexl;
                scaleDst[i * scaleLineByte + px + 1] = piexl;
                scaleDst[i * scaleLineByte + px + 2] = piexl;
                px += 3;
            }
        }
    }
    // scale pic data
    // 就近插值算法
    unsigned int lineByte = alloc_line_width(24, newWidth);
    unsigned char *dsts;
    dsts = (unsigned char *)malloc(lineByte * newHeight);
    memset(dsts, 0, lineByte * newHeight);
    for (int i = 0; i < newHeight; i++)
    {

        int x = round(i * 1.0f * height / newHeight);
        for (int j = 0; j < lineByte; j++)
        {
            int y = round(j * 1.0f * scaleLineByte / lineByte);
            int b = scaleDst[x * scaleLineByte + y];
            int g = scaleDst[x * scaleLineByte + y + 1];
            int r = scaleDst[x * scaleLineByte + y + 2];
            dsts[i * lineByte + j] = b;
            dsts[i * lineByte + j + 1] = g;
            dsts[i * lineByte + j + 2] = r;
            j += 2;
        }
    }

    free(scaleDst);

    // 24bpp to 1bpp
    unsigned int lineByte_8 = alloc_line_width(1, newWidth);
    unsigned char *dsts_8;
    dsts_8 = (unsigned char *)malloc(lineByte_8 * newHeight);
    memset(dsts_8, 0, lineByte_8 * newHeight);
    for (int i = 0; i < newHeight; i++)
    {
        int ix = 0;
        for (int j = 0; j < lineByte; j++)
        {
            int d = dsts[i * lineByte + j];
            int p = d > 128 ? 1 : 0;
            int rj = j / 3;
            ix = rj / 8;
            dsts_8[i * lineByte_8 + ix] |= (p << (7 - rj % 8));
            j += 2;
        }
    }
    free(dsts);

    //修正bmp数据结构内容
    bmpFileHeader.bfSize = lineByte_8 * newHeight + bmpFileHeader.bfOffBits;
    bmpInfoHeader.biHeight = newHeight;
    bmpInfoHeader.biWidth = newWidth;
    bmpInfoHeader.biSizeImage = lineByte_8 * newHeight;
    bmpInfoHeader.biBitcount = 1;

    bmpFile->fileHeader = bmpFileHeader;
    bmpFile->data = dsts_8;
    bmpFile->infoHeader = bmpInfoHeader;
    bmpFile->palette = palette;
}