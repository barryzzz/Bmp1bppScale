#include "bmp.h"
#include <iostream>
#include <string.h>
using namespace std;

#define LOG(fmt, ...) printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__)

#define IMG_WIDTH 160
#define IMG_HEIGHT 160
#define IMG_FILE_INPUT_PATH "../test.jpg"
#define IMG_FILE_OUT_PUT_PATH "../test.bmp"

int main()
{

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
    bmpfileHeader.bfOffBits = sizeof(bmpfileHeader) + sizeof(bmpInfoHeader) + sizeof(palette);
    LOG("bmpfileHeader:%u", sizeof(bmpfileHeader));
    LOG("bmpInfoHeader:%u", sizeof(bmpInfoHeader));
    LOG("palette:%u", sizeof(palette));
    unsigned long bmpLineWidth = (((IMG_WIDTH * 1) + 31) & ~31) / 8;
    bmpfileHeader.bfSize = bmpfileHeader.bfOffBits + bmpLineWidth * IMG_HEIGHT;

    //图片信息头配置
    bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth = IMG_WIDTH;
    bmpInfoHeader.biHeight = IMG_HEIGHT;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitcount = 1;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.biSizeImage = bmpLineWidth * IMG_HEIGHT;
    bmpInfoHeader.biXpelsPermeter = 0;
    bmpInfoHeader.biYPelsPermeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    unsigned char *src;
    unsigned char *dst;

    FILE *finput;
    FILE *fout;
    finput = fopen(IMG_FILE_INPUT_PATH, "rb");

    src = (unsigned char *)malloc(IMG_WIDTH * IMG_HEIGHT);
    dst = (unsigned char *)malloc(IMG_WIDTH * IMG_HEIGHT / 8);

    if (NULL == src)
    {
        LOG("malloc error!%d", IMG_WIDTH * IMG_HEIGHT);
        fclose(finput);
        return -1;
    }

    memset(src, 0, (IMG_WIDTH * IMG_HEIGHT));
    fread(src, IMG_WIDTH * IMG_HEIGHT, 1, finput);
    fclose(finput);

    memset(dst, 0, (IMG_WIDTH * IMG_HEIGHT / 8));

    int index = 0;
    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            // unsigned char pixel = src[y * IMG_WIDTH + x];
            // if (pixel > 128)
            // {
            dst[index / 8] |= (1 << (7 - (index % 8)));
            // }
            // else
            // {
            // LOG("not >> %d", 0);
            // }
            index++;
        }
    }
    fout = fopen(IMG_FILE_OUT_PUT_PATH, "wb");
    fwrite(&bmpfileHeader, sizeof(bmpfileHeader), 1, fout);
    fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fout);
    fwrite(&palette, sizeof(palette), 1, fout);
    fwrite(dst, IMG_WIDTH * IMG_HEIGHT / 8, 1, fout);

    free(dst);
    free(src);

    return 0;
}