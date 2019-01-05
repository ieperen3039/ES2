

#include <stdio.h>
#include "preprocessing.h"

void cpu_preprocess(BLOB* img) {
    //Subtract mean RGB values, scale with 0.017, and swap RGB->BGR
    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            float R = (blob_data(img, 0, y, x) - 123.680f) * 0.017f;
            float G = (blob_data(img, 1, y, x) - 116.779f) * 0.017f;
            float B = (blob_data(img, 2, y, x) - 103.939f) * 0.017f;
            blob_data(img, 0, y, x) = B;
            blob_data(img, 1, y, x) = G;
            blob_data(img, 2, y, x) = R;
        }
    }
}

