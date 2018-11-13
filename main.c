#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_util.h"
#include "network.h"
#include "mobilenetv2.h"

void preprocess(IMG* img){
    //Subtract mean RGB values, scale with 0.017, and swap RGB->BGR
    for(int y=0;y<img->height;y++)
        for(int x=0;x<img->width;x++){
            float R            = (img->data[0][y][x]-123.680f)*0.017f; //R
            img->data[1][y][x] = (img->data[1][y][x]-116.779f)*0.017f; //G
            img->data[0][y][x] = (img->data[2][y][x]-103.939f)*0.017f; //B
            img->data[2][y][x] = R;
         }
}

int main(int argc, char* argv[]){
    if (argc!=2 || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")){
        fprintf(stderr, "Usage: %s <input.png>\n", argv[0]);
        exit(-1);
    }

    //read png into image structure
    printf("Loading image %s\n",argv[1]);
    IMG* img = read_png(argv[1]);

    //Do preprocessing of the image
    printf("Preprocessing image\n");
    preprocess(img);

    //perform inference
    printf("Performing inference\n");
    BLOB* out = network(&mobilenetv2, img);
    for(int i=0;i<5;i++)
        printf("result %f\n",out->data[0][0][i]);
    printf("d:%d h:%d w:%d\n", out->d, out->h, out->w);

    //cleanup image
    destroy_img(img);

    return 0;
}
