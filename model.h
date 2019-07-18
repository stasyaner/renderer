#pragma once

#include "geometry.h"

#define MAXSTR 100
#define VERTEX_NUMBER_STEP 500
#define FACE_NUMBER_STEP 500
#define NORMAL_NUMBER_STEP 500

typedef Vec3f VERTEX;

typedef struct {
        int v0, v1, v2;
} FACE;

typedef struct {
        VERTEX *vertices;
        int vnum;
        FACE *faces;
        int fnum;
        VERTEX *normals;
        int nnum;
} MODEL;

int load_model();
void free_model();
int draw_model();
