#include <stdio.h>
#include <stdlib.h>
#include "model.h"

int main() {
        load_model();
        draw_model();
        free_model();
        return 0;
}
