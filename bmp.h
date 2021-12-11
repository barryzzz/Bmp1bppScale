#ifndef BMP_H
#define BMP_H

#endif // BMP_H

// #pragma pack(2)
// 文件头
struct BMPFileHeader
{
    unsigned short bfType;
    unsigned long bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long bfOffBits;
};

// Bmp信息头
struct BMPInfoHeader
{
    unsigned long biSize;
    long biWidth;
    long biHeight;
    unsigned short biPlanes;
    unsigned short biBitcount;
    unsigned long biCompression;
    unsigned long biSizeImage;
    long biXpelsPermeter;
    long biYPelsPermeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
};

// bmp颜色表
struct BMPRgbQuad
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};