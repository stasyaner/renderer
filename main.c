#include <stdio.h>
#include <stdlib.h>
#include "tga.h"
#include "model.h"

#include <limits.h>

int main() {
        load_model();
        draw_model();
        free_model();
        return 0;
}
