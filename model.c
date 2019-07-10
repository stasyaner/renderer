#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "tga.h"

static MODEL model = {
        .vertices = NULL,
        .vnum = 0,
        .faces = NULL,
        .fnum = 0
};

int load_model()
{
        char ts[MAXSTR];
        float x, y;
        int v0, v1, v2;
        int vlimit = VERTEX_NUMBER_STEP;
        int flimit = FACE_NUMBER_STEP;
        model.vertices = (VERTEX *) malloc(VERTEX_NUMBER_STEP * sizeof(VERTEX));
        model.faces = (FACE *) malloc(FACE_NUMBER_STEP * sizeof(FACE));
        FILE *tf = fopen("/mnt/c/Users/stasyaner/Downloads/african_head.obj", "r");
        while(!feof(tf))
        {
                fgets(ts, MAXSTR, tf);
                if(ts[0] == 'v' && ts[1] == ' ') {
                        sscanf(ts, "%*s %f %f", &x, &y);
                        if(model.vnum == vlimit - 1)
                                model.vertices = (VERTEX *) realloc(model.vertices, (vlimit += VERTEX_NUMBER_STEP) * sizeof(VERTEX));
                        model.vertices[model.vnum].x = x;
                        model.vertices[model.vnum++].y = y;
                } else if(ts[0] == 'f') {
                        sscanf(ts, "%*s %d/%*d/%*d %d/%*d/%*d %d", &v0, &v1, &v2);
                        if(model.fnum == flimit - 1)
                                model.faces = (FACE *) realloc(model.faces, (flimit += FACE_NUMBER_STEP) * sizeof(FACE));
                        model.faces[model.fnum].v0 = v0;
                        model.faces[model.fnum].v1 = v1;
                        model.faces[model.fnum++].v2 = v2;
                }
        }
        fclose(tf);
        return 0;
}

int draw_model() {
        int width = 800;
        int height = 800;
        init_tga_data(width, height);
        for(int i = 0; i < model.fnum; i++) {
                FACE v = model.faces[i];
                int x0 = (model.vertices[v.v0-1].x + 1.) * width / 2.;
                int y0 = (model.vertices[v.v0-1].y + 1.) * height / 2.;
                int x1 = (model.vertices[v.v1-1].x + 1.) * width / 2.;
                int y1 = (model.vertices[v.v1-1].y + 1.) * height / 2.;
                int x2 = (model.vertices[v.v2-1].x + 1.) * width / 2.;
                int y2 = (model.vertices[v.v2-1].y + 1.) * height / 2.;
                line(x0, y0, x1, y1, COLOR_WHITE);
                line(x1, y1, x2, y2, COLOR_WHITE);
                line(x2, y2, x0, y0, COLOR_WHITE);
        }
        write_free_tga("/mnt/c/Users/stasyaner/Desktop/african_head.tga");
        return 0;
}

void free_model() {
        free(model.vertices);
        model.vnum = 0;
        free(model.faces);
        model.fnum = 0;
}
