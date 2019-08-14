#pragma once

#include "geometry.h"
#include "tga.h"

#define MAXSTR                100
#define VERTEX_NUMBER_STEP    500
#define FACE_NUMBER_STEP      500
#define NORMAL_NUMBER_STEP    500
#define VTEXTURE_NUMBER_STEP  500

typedef struct {
        int v0, v1, v2, vt0, vt1, vt2;
} FACE;

typedef struct {
        Vec3f *vertices;
        int vnum;
        FACE *faces;
        int fnum;
        Vec3f *normals;
        int nnum;
        TGAIMG texture;
        Vec3f *vtexture;
        int vtnum;
} MODEL;

int load_model();
void free_model();
int draw_model();
