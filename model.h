#pragma once

#include "geometry.h"
#include "tga.h"

#define MAXSTR                100
#define VERTEX_NUMBER_STEP    500
#define FACE_NUMBER_STEP      500
#define NORMAL_NUMBER_STEP    500
#define VTEXTURE_NUMBER_STEP  500

typedef Vec3f VERTEX;

typedef struct {
        int v0, v1, v2, vt0, vt1, vt2;
} FACE;

typedef struct {
        VERTEX *vertices;
        int vnum;
        FACE *faces;
        int fnum;
        VERTEX *normals;
        int nnum;
        TGAIMG texture;
        VERTEX *vtexture;
        int vtnum;
} MODEL;

int load_model();
void free_model();
int draw_model();
