#include "image.h"
#include <stdio.h>

Initialazation::Initialazation(const char *file_name) {
    FILE *stream = fopen(file_name, "rb");
    fread(&FileHeader, sizeof(FileHeader), 1, stream);
    fread(&InfoHeader, sizeof(InfoHeader), 1, stream);
    fseek(stream, FileHeader.bfOffBits, SEEK_SET);
    bgr = static_cast<unsigned char *>(malloc(FileHeader.bfSize - FileHeader.bfOffBits));
    fread(bgr, FileHeader.bfSize - FileHeader.bfOffBits, 1, stream);
    fclose(stream);
    auto koef = ImageFilters::Koeff(*this);
    image_ = std::vector<std::vector<BGR>>(InfoHeader.biHeight, std::vector<BGR>(InfoHeader.biWidth));
    for (LONG i = 0; i < InfoHeader.biHeight; ++i) {
        for (LONG j = 0; j < InfoHeader.biWidth; ++j) {
            image_[i][j].blue = bgr[i * koef + 3 * j];
            image_[i][j].green = bgr[i * koef + 3 * j + 1];
            image_[i][j].red = bgr[i * koef + 3 * j + 2];
        }
    }
    free(bgr);
}

void Initialazation::WriteImage(const char *file_name) {
    FILE *outfile = fopen(file_name, "wb");
    fwrite(&FileHeader, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
    fwrite(&InfoHeader, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
    fwrite(NormalFormat(), sizeof(unsigned char), FileHeader.bfSize - FileHeader.bfOffBits, outfile);
    fclose(outfile);
    free(bgr);
}

int ImageFilters::Koeff(Initialazation &image) {
    return (4 - (image.InfoHeader.biWidth * 3) % 4) % 4 + 3 * image.InfoHeader.biWidth;
}

unsigned char *Initialazation::NormalFormat() {
    bgr = static_cast<unsigned char *>(malloc(InfoHeader.biSizeImage));
    int koef = FilterCrop::Koeff(*this);
    for (size_t i = 0; i < InfoHeader.biSizeImage; ++i) {
        bgr[i] = 0;
    }
    for (LONG i = 0; i < InfoHeader.biHeight; ++i) {
        for (LONG j = 0; j < InfoHeader.biWidth; ++j) {
            bgr[i * koef + 3 * j] = image_[i][j].blue;
            bgr[i * koef + 3 * j + 1] = image_[i][j].green;
            bgr[i * koef + 3 * j + 2] = image_[i][j].red;
        }
    }
    return bgr;
}

void FilterCrop::Crop(Initialazation &image, int width, int height) {
    if (height > image.InfoHeader.biHeight) {
        height = image.InfoHeader.biHeight;
    }
    if (width > image.InfoHeader.biWidth) {
        width = image.InfoHeader.biWidth;
    }

    std::vector<std::vector<BGR>> part(height, std::vector<BGR>(width));
    for (LONG i = 0; i < height; ++i) {
        for (LONG j = 0; j < width; ++j) {
            part[height - i - 1][j] = image.image_[image.InfoHeader.biHeight - i - 1][j];
        }
    }
    image.image_ = part;
    image.InfoHeader.biWidth = width;
    image.InfoHeader.biHeight = height;
    image.InfoHeader.biSizeImage = Koeff(image) * image.InfoHeader.biHeight;
    image.FileHeader.bfSize = image.InfoHeader.biSizeImage + int54;
}

void FilterGrayscale::Grayscale(Initialazation &image) {
    for (LONG i = 0; i < image.InfoHeader.biHeight; ++i) {
        for (LONG j = 0; j < image.InfoHeader.biWidth; ++j) {
            int color = image.image_[i][j].blue * int114 / int1000 + image.image_[i][j].green * int587 / int1000 +
                        image.image_[i][j].red * int299 / int1000;
            image.image_[i][j].blue = color;
            image.image_[i][j].green = color;
            image.image_[i][j].red = color;
        }
    }
}

void FilterNegative::Negative(Initialazation &image) {
    for (LONG i = 0; i < image.InfoHeader.biHeight; ++i) {
        for (LONG j = 0; j < image.InfoHeader.biWidth; ++j) {
            image.image_[i][j].blue = uchar255 - image.image_[i][j].blue;
            image.image_[i][j].green = uchar255 - image.image_[i][j].green;
            image.image_[i][j].red = uchar255 - image.image_[i][j].red;
        }
    }
}
