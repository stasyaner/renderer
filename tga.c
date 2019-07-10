#include <stdio.h>
#include <stdlib.h>
#include "tga.h"

static PIXEL *pixels;
static TGAIMG tgaimg;
static int width = 100;
static int height = 100;
static int pixeln = 10000;

int init_tga_data(int w, int h)
{
        if(w <= 0 || h <= 0) return 1;
        width = w;
        height = h;
        pixeln = w * h;
        pixels = (PIXEL *) malloc(pixeln * sizeof(PIXEL));
        for(int i = 0; i < pixeln; i++) {
                pixels[i].r = 0;
                pixels[i].g = 0;
                pixels[i].b = 0;
                pixels[i].a = 255;
        }
        tgaimg.data = pixels;

        return 1;
}

int set_pixel(int x, int y, PIXEL color, _Bool steep)
{
        if(steep) swapcrd(x, y);
        if(tgaimg.data == NULL) return 1;
        tgaimg.data[x + y * width].r = color.r;
        tgaimg.data[x + y * width].g = color.g;
        tgaimg.data[x + y * width].b = color.b;
        tgaimg.data[x + y * width].a = 255;
        return 0;
}

int line(int x0, int y0, int x1, int y1, PIXEL color)
{
        if(tgaimg.data == NULL) return 1;
        _Bool steep = abs(y1 - y0) > abs(x1 - x0);
        if(steep) swapax(x0, y0, x1, y1);
        if(x0 > x1) swapax(x0, x1, y0, y1);
        int dy = abs(y1 - y0);
        int dx = abs(x1 - x0);
        int err = -dx;
        for (int x = x0, y = y0; x <= x1; x++) {
                set_pixel(x, y, color, steep);
                err += 2 * dy;
                if (err >= 0) {
                        y1 > y0 ? y++ : y--;
                        err -= 2 * dx;
                }
        }
        return 0;
}

int write_free_tga(char *filename)
{
        FILE *tgaf = fopen(filename, "w");
        if(tgaf == NULL) return 1;

        putc(0, tgaf);
        putc(0, tgaf);
        putc(2, tgaf);
        putc(0, tgaf); putc(0, tgaf);
        putc(0, tgaf); putc(0, tgaf);
        putc(0, tgaf);
        putc(0, tgaf); putc(0, tgaf);
        putc(0, tgaf); putc(0, tgaf);
        putc(width & 0x00FF, tgaf);
        putc((width & 0xFF00) / 256, tgaf);
        putc(height & 0x00FF, tgaf);
        putc((height & 0xFF00) / 256, tgaf);
        putc(32, tgaf);
        putc(0, tgaf);

        for(int i = 0; i < pixeln; i++) {
                putc(tgaimg.data[i].b, tgaf);
                putc(tgaimg.data[i].g, tgaf);
                putc(tgaimg.data[i].r, tgaf);
                putc(tgaimg.data[i].a, tgaf);
        }

        fclose(tgaf);
        free(pixels);
        return 0;
}
