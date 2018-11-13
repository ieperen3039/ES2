#include <stdlib.h>
#include <string.h>
#include "blob.h"

//generic blob allocation which can be called with malloc or calloc as allocation function
BLOB* __alloc_blob(void*(*alloc)(size_t size), int d, int h, int w){
    BLOB* b=(BLOB*)(*alloc)(sizeof(BLOB));
    b->w=w;
    b->h=h;
    b->d=d;
    b->data=(float***) (*alloc)(sizeof(float**)*d);
    for(int z=0;z<d;z++){
        b->data[z]=(float**) (*alloc)(sizeof(float*)*h);
        for(int y=0;y<h;y++)
            b->data[z][y]= (float*) (*alloc)(sizeof(float)*w);
    }
    return b;
};

//custom calloc which matches the "malloc" signature
void* my_calloc(size_t size){
    return calloc(1, size);
}

//allocate blob with given dimensions
BLOB* alloc_blob(int d, int h, int w){
    return __alloc_blob(&malloc,d,h,w);
}

//calloc blob with given dimensions (zero init)
BLOB* calloc_blob(int d, int h, int w){
    return __alloc_blob(&my_calloc,d,h,w);
}

//free blob
void free_blob(BLOB* b){
    for(int z=0;z<b->d;z++){
        for(int y=0;y<b->h;y++)
            free(b->data[z][y]);
        free(b->data[z]);
    }
    free(b->data);
}

//duplicate blob
BLOB* duplicate_blob(BLOB* b){
    BLOB* out = alloc_blob(b->d, b->h, b->w);
    for(int z=0;z<out->d;z++)
        for(int y=0;y<out->h;y++)
            memcpy(out->data[z][y], b->data[z][y], sizeof(float)*b->w);
    return out;
}
