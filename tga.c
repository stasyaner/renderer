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

BARCOORD _comp_bar_coord(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
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

int triangle(Vec3f v0, Vec3f v1, Vec3f v2, float *zbuf, PIXEL color)
{
        if(tgaimg.data == NULL) return 1;
        int minx = min(v0.x, v1.x, v2.x);
        int maxx = max(v0.x, v1.x, v2.x);
        int miny = min(v0.y, v1.y, v2.y);
        int maxy = max(v0.y, v1.y, v2.y);
        BARCOORD bc;
        Vec3f p;
        int zidx;
        for(p.x = minx; p.x <= maxx; p.x++) {
                for(p.y = miny; p.y <= maxy; p.y++) {
                        bc = _comp_bar_coord(v0, v1, v2, (Vec3f) {p.x, p.y});
                        if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
                        zidx = p.x + p.y * 800;
                        p.z  = v0.x * bc.x + v1.x * bc.y + v2.x * bc.z;
                        p.z += v1.y * bc.x + v1.y * bc.y + v2.y * bc.z;
                        p.z += v2.z * bc.x + v2.z * bc.y + v2.z * bc.z;
                        if (zbuf[zidx] > p.z) continue;
                        zbuf[zidx] = p.z;
                        set_pixel(p.x, p.y, color, 0);
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
