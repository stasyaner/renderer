#include <stdio.h>
#include <stdlib.h>
#include "tga.h"
#include <math.h>
#include "geometry.h"

static PIXEL *pixels;
static TGAIMG tgaimg;
static int width = 100;
static int height = 100;
static int pixeln = 10000;

/* _Bool _is_ins_tri(int x, int y, Vec3f v0, Vec3f v1, Vec3f v2)
{
        _Bool st0 = signbit((v0.x - x) * (v1.y - v0.y) - (v1.x - v0.x) * (v0.y - y));
        _Bool st1 = signbit((v1.x - x) * (v2.y - v1.y) - (v2.x - v1.x) * (v1.y - y));
        _Bool st2 = signbit((v2.x - x) * (v0.y - v2.y) - (v0.x - v2.x) * (v2.y - y));
        if ((st0 && st1 && st2) || (!st0 && !st1 && !st2)) return 1;
        return 0;
} */

BARCOORD _comp_bar_coord(Vec3f a, Vec3f b, Vec3f c, Vec2i p)
{
        float m1u = (p.y-c.y)*(b.x-c.x)-(p.x-c.x)*(b.y-c.y);
        float m1d = (a.y-c.y)*(b.x-c.x)-(a.x-c.x)*(b.y-c.y);
        float m2u = (p.y-a.y)*(c.x-a.x)-(p.x-a.x)*(c.y-a.y);
        float m2d = (b.y-a.y)*(c.x-a.x)-(b.x-a.x)*(c.y-a.y);
        float m3u = (p.y-a.y)*(b.x-a.x)-(p.x-a.x)*(b.y-a.y);
        float m3d = (c.y-a.y)*(b.x-a.x)-(c.x-a.x)*(b.y-a.y);
        return (BARCOORD) {m1u/m1d, m2u/m2d, m3u/m3d};
}

/* BARCOORD _comp_bar_coord2(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
{
        Vec3f xs = {c.x - a.x, b.x - a.x, a.x - p.x};
        Vec3f ys = {c.y - a.y, b.y - a.y, a.y - p.y};
        Vec3f u = {xs.y*ys.z - xs.z*ys.y,
                   xs.z*ys.x - xs.x*ys.z,
                   xs.x*ys.y - xs.y*ys.x};
        return (BARCOORD) {1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z};
} */

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

int triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f vt0, Vec3f vt1, Vec3f vt2,
             float *zbuf, TGAIMG texture, float intensity)
{
        if(tgaimg.data == NULL) return 1;
        float minx = min(v0.x, v1.x, v2.x);
        float maxx = max(v0.x, v1.x, v2.x);
        float miny = min(v0.y, v1.y, v2.y);
        float maxy = max(v0.y, v1.y, v2.y);
        BARCOORD vbc, tbc;
        float zval;
        int zidx;
        PIXEL color = {255, 255, 255, 255};
        float r1;
        for(int x = minx; x <= maxx; x++) {
                for(int y = miny; y <= maxy; y++) {
                        r1 = (v1.z - v0.z) / (maxx - minx) * (x - minx) + v0.z;
                        // r1 = (v1.z - v0.z) / (v1.x - v0.x) * (x - v0.x) + v0.z;
                        // r1 = (maxx - x) / (maxx - minx) * v0.z + (x - minx) / (maxx - minx) * v1.z;
                        // r2 = v2.z;
                        // zval = (v2.z - r1) / (maxy - miny) * (y - miny) + r1;
                        // zval = (v2.z - r1) / (v2.y - v0.y) * (y - v0.y) + r1;
                        // zval  = (maxy - y) / (maxy - miny) * r1   + (y - miny) / (maxy - miny) * v2.z;
                        vbc = _comp_bar_coord(v0, v1, v2, (Vec2i) {x, y});
                        if (vbc.x < 0 || vbc.y < 0 || vbc.z < 0) continue;
                        zidx = x + y * width;
                        zval = v0.z * vbc.x + v1.z * vbc.y + v2.z * vbc.z;
                        // printf("x=%d, y=%d, zval=%.10f, r1=%.10f, r2=%.10f, p=%.10f, p1=%.10f\n", x, y, zval, r1, r2, p, p1);
                        if (zbuf[zidx] > zval) continue;
                        zbuf[zidx] = zval;
                        tbc = _comp_bar_coord(vt0, vt1, vt2, (Vec2i) {x, y});
                        // int tx =
                        color = COLOR_WHITE;// get_pixel(x, y, texture);
                        color.r = color.r * intensity;
                        color.g = color.g * intensity;
                        color.b = color.b * intensity;
                        set_pixel(x, y, color, 0);
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

TGAIMG read_tga(char *filename)
{
        TGAIMG tgaimg;
        FILE *tgaf = fopen(filename, "r");

        tgaimg.header.idlength = fgetc(tgaf);
        tgaimg.header.colourmaptype = fgetc(tgaf);
        tgaimg.header.datatypecode = fgetc(tgaf);
        fread(&tgaimg.header.colourmaporigin, 2, 1, tgaf);
        fread(&tgaimg.header.colourmaplength, 2, 1, tgaf);
        tgaimg.header.colourmapdepth = fgetc(tgaf);
        fread(&tgaimg.header.x_origin, 2, 1, tgaf);
        fread(&tgaimg.header.y_origin, 2, 1, tgaf);
        fread(&tgaimg.header.width, 2, 1, tgaf);
        fread(&tgaimg.header.height, 2, 1, tgaf);
        tgaimg.header.bitsperpixel = fgetc(tgaf);
        tgaimg.header.imagedescriptor = fgetc(tgaf);

        size_t imgdatasize = tgaimg.header.width *
                 tgaimg.header.height * sizeof(PIXEL);
        tgaimg.data = (PIXEL *) malloc(imgdatasize);
        fread(tgaimg.data, imgdatasize, 1, tgaf);

        fclose(tgaf);
        return tgaimg;
}
