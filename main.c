#include <stdio.h>
#include <stdlib.h>
#include "tga.h"
#include "model.h"

int main() {
        load_model();
        draw_model();
        free_model();
        // init_tga_data(20, 20);
        // triangle((VERTEX) {3, 3}, (VERTEX) {15, 5}, (VERTEX) {7, 15}, NULL, COLOR_WHITE);
        // write_free_tga("/mnt/c/Users/stasyaner/Desktop/bartri1.tga");
        return 0;
}
