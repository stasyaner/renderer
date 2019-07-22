#include <stdio.h>
#include <stdlib.h>
#include "tga.h"
#include "model.h"

#include <limits.h>

int main() {
        load_model();
        draw_model();
        free_model();
        // float zbuf[20 * 20];
        // for (int i = 20 * 20; i--; zbuf[i] = INT_MIN);
        // init_tga_data(20, 20);
        // triangle((VERTEX) {3, 3, 3}, (VERTEX) {15, 5, 5}, (VERTEX) {7, 15, 8},
        // (VERTEX) {0, 0, 0}, (VERTEX) {0, 0, 0}, (VERTEX) {0, 0, 0},
        // zbuf, (TGAIMG) { (TGAH) {0}, NULL}, 1);
        // write_free_tga("/mnt/c/Users/stasyaner/Desktop/interpol1.tga");
        return 0;
}
