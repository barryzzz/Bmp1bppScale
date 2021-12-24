#ifndef BMP_H
#define BMP_H

#define alloc_line_width(bit, width) ((((width * bit) + 31) & ~31) / 8);
#define LOG(fmt, ...) printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__)

#pragma pack(2)
// 文件头
struct BMPFileHeader
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

// Bmp信息头
struct BMPInfoHeader
{
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitcount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXpelsPermeter;
    int biYPelsPermeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

// bmp颜色表
struct BMPRgbQuad
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};

struct BMPFile
{
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    BMPRgbQuad *palette;
    unsigned char *data;
};

#pragma pack()

#endif // BMP_H