#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

using WORD = uint16_t;
using DWORD = uint32_t;
using LONG = int32_t;

using BITMAPFILEHEADER = struct __attribute__((packed)) TagBitmapfileheader {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
};

using BITMAPINFOHEADER = struct __attribute__((packed)) TagBitmapinfoheader {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

struct BGR {
    unsigned char blue = 0;
    unsigned char green = 0;
    unsigned char red = 0;

    BGR(unsigned char b, unsigned char g, unsigned char r) {
        blue = b;
        green = g;
        red = r;
    }

    BGR() = default;
};

class Initialazation {
public:
    unsigned char *bgr;
    BITMAPINFOHEADER InfoHeader;
    BITMAPFILEHEADER FileHeader;
    std::vector<std::vector<BGR>> image_;
    unsigned char *NormalFormat();

    explicit Initialazation(const char *);
    void WriteImage(const char *);
};

class ImageFilters {
public:
    static int Koeff(Initialazation &);
    const float num255 = 255;
    const int int54 = 54;
    const int int1000 = 1000;
    const int int114 = 114;
    const int int587 = 587;
    const int int299 = 299;
    const unsigned char uchar255 = 255;
    virtual ~ImageFilters() = default;
};

class FilterCrop : public ImageFilters {
public:
    void Crop(Initialazation &, int, int);
};

class FilterGrayscale : public ImageFilters {
public:
    void Grayscale(Initialazation &);
};

class FilterNegative : public ImageFilters {
public:
    void Negative(Initialazation &);
};