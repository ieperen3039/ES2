#ifndef BLOB_H
#define BLOB_H

//Basic blob datastructure
typedef struct {
    float*** data;
    int w; //width
    int h; //height
    int d; //depth
} BLOB;

//malloc blob with given dimensions (zero init)
BLOB* alloc_blob(int d, int h, int w);

//calloc blob with given dimensions (zero init)
BLOB* calloc_blob(int d, int h, int w);

//free blob
void free_blob(BLOB* b);

//duplicate blob
BLOB* duplicate_blob(BLOB* b);

#endif
