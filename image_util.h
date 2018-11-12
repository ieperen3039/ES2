#ifndef IMAGE_UTIL_H
#define IMAGE_UTIL_H

#include <stdio.h>
#include <stdbool.h>

//structure to hold images
typedef struct {
    int channels;
    int width;
    int height;
    //data[channel][y][x]; (channels=RGB)
    float*** data;
} IMG;

IMG* read_png(const char *filename);
bool write_png(const char *filename, IMG* img);
void destroy_img(IMG* img);

#endif
