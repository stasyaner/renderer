#include <stdio.h>
#include <stdlib.h>
#include "tga.h"
#include "model.h"

#define max(a,b,c) (a > b ? (a > c ? a : c) : (b > c ? b : c))
#define min(a,b,c) (a < b ? (a < c ? a : c) : (b < c ? b : c))

_Bool is_ins_tri(int x, int y, VERTEX v0, VERTEX v1, VERTEX v2);

int main() {
        init_tga_data(100, 100);
        VERTEX v0 = {10, 10};
        VERTEX v1 = {30, 80};
        VERTEX v2 = {90, 30};
        line(v0.x, v0.y, v1.x, v1.y, COLOR_WHITE);
        line(v0.x, v0.y, v2.x, v2.y, COLOR_WHITE);
        line(v1.x, v1.y, v2.x, v2.y, COLOR_WHITE);
        int minx = min(v0.x, v1.x, v2.x);
        int maxx = max(v0.x, v1.x, v2.x);
        int miny = min(v0.y, v1.y, v2.y);
        int maxy = max(v0.y, v1.y, v2.y);
        for(int x = minx; x <= maxx; x++) {
                for(int y = miny; y <= maxy; y++) {
                        if (is_ins_tri(x, y, v0, v1, v2))
                                set_pixel(x, y, COLOR_WHITE, 0);
                }
        }
        write_free_tga("/mnt/c/Users/stasyaner/Desktop/triangle1.tga");
        return 0;
}

_Bool is_ins_tri(int x, int y, VERTEX v0, VERTEX v1, VERTEX v2) {
        int st0 = (v0.x - x) * (v1.y - v0.y) - (v1.x - v0.x) * (v0.y - y);
        int st1 = (v1.x - x) * (v2.y - v1.y) - (v2.x - v1.x) * (v1.y - y);
        int st2 = (v2.x - x) * (v0.y - v2.y) - (v0.x - v2.x) * (v2.y - y);
        if ((st0 > 0 && st1 > 0 && st2 > 0) ||
            (st0 < 0 && st1 < 0 && st2 < 0)) return 1;
        return 0;
}
