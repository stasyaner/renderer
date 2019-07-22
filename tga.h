#pragma once

#include "geometry.h"

#define COLOR_WHITE  (PIXEL) {255, 255, 255, 255}
#define COLOR_RED    (PIXEL) {255,   0,   0, 255}

#define swapcrd(x, y) do {int t = x; x = y; y = t;} while(0)
#define swapax(a, b, c, d) do {swapcrd(a, b); swapcrd(c, d);} while(0)
#define max(a,b,c) (a > b ? (a > c ? a : c) : (b > c ? b : c))
#define min(a,b,c) (a < b ? (a < c ? a : c) : (b < c ? b : c))
#define get_pixel(x, y, img) img.data[x + y * img.header.width]

typedef struct {
        unsigned char r, g, b, a;
} PIXEL;

typedef Vec3f BARCOORD;

typedef struct {
        char  idlength;
        char  colourmaptype;
        char  datatypecode;
        short colourmaporigin;
        short colourmaplength;
        char  colourmapdepth;
        short x_origin;
        short y_origin;
        short width;
        short height;
        char  bitsperpixel;
        char  imagedescriptor;
} TGAH;

typedef struct {
        TGAH header;
        PIXEL *data;
} TGAIMG;

int set_pixel(int x, int y, PIXEL color, _Bool steep);
int line(int x0, int y0, int x1, int y1, PIXEL color);
int triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f vt0, Vec3f vt1, Vec3f vt2,
             float *zbuf, TGAIMG texture, float intensity);
int write_free_tga(char *filename);
int init_tga_data(int w, int h);
TGAIMG read_tga(char *filename);
