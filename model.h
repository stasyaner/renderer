#pragma once

#define MAXSTR 100
#define VERTEX_NUMBER_STEP 500
#define FACE_NUMBER_STEP 500

typedef struct {
        float x, y;
} VERTEX;

typedef struct {
        int v0, v1, v2;
} FACE;

typedef struct {
        VERTEX *vertices;
        int vnum;
        FACE *faces;
        int fnum;
} MODEL;

int load_model();
void free_model();
int draw_model();
