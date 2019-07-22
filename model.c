#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "tga.h"
#include <math.h>
#include <limits.h>

static MODEL model = {
        .vnum = 0,
        .fnum = 0,
        .vtnum = 0
};

int load_model()
{
        char ts[MAXSTR];
        float x, y, z;
        int v0, v1, v2, vt0, vt1, vt2;
        int vlimit = VERTEX_NUMBER_STEP;
        int flimit = FACE_NUMBER_STEP;
        int vtlimit = VTEXTURE_NUMBER_STEP;
        model.vertices = (VERTEX *) malloc(VERTEX_NUMBER_STEP * sizeof(VERTEX));
        model.faces = (FACE *) malloc(FACE_NUMBER_STEP * sizeof(FACE));
        model.vtexture = (VERTEX *) malloc(VTEXTURE_NUMBER_STEP * sizeof(VERTEX));
        FILE *tf = fopen("/mnt/c/Users/stasyaner/Downloads/african_head.obj", "r");
        while(!feof(tf))
        {
                fgets(ts, MAXSTR, tf);
                if(ts[0] == 'v' && ts[1] == ' ') {
                        sscanf(ts, "%*s %f %f %f", &x, &y, &z);
                        if(model.vnum == vlimit - 1)
                                model.vertices = (VERTEX *) realloc(model.vertices,
                                (vlimit += VERTEX_NUMBER_STEP) * sizeof(VERTEX));
                        model.vertices[model.vnum].x = x;
                        model.vertices[model.vnum].y = y;
                        model.vertices[model.vnum++].z = z;
                } else if(ts[0] == 'f') {
                        sscanf(ts, "%*s %d/%d/%*d %d/%d/%*d %d/%d", &v0, &vt0, &v1, &vt1, &v2, &vt2);
                        if(model.fnum == flimit - 1)
                                model.faces = (FACE *) realloc(model.faces,
                                (flimit += FACE_NUMBER_STEP) * sizeof(FACE));
                        model.faces[model.fnum].v0 = v0;
                        model.faces[model.fnum].v1 = v1;
                        model.faces[model.fnum].v2 = v2;
                        model.faces[model.fnum].vt0 = vt0;
                        model.faces[model.fnum].vt1 = vt1;
                        model.faces[model.fnum++].vt2 = vt2;
                } else if(ts[0] == 'v' && ts[1] == 't') {
                        sscanf(ts, "%*s %f %f %f", &x, &y, &z);
                        if(model.vtnum == vtlimit - 1)
                                model.vtexture = (VERTEX *) realloc(model.vtexture,
                                (vtlimit += VTEXTURE_NUMBER_STEP) * sizeof(VERTEX));
                        model.vtexture[model.vtnum].x = x;
                        model.vtexture[model.vtnum].y = y;
                        model.vtexture[model.vtnum++].z = z;
                }
        }
        model.texture = read_tga("/mnt/c/Users/stasyaner/Downloads/"
                                 "african_head_diffuse_no_rle.tga");
        fclose(tf);
        return 0;
}

int draw_model() {
        int width = 800;
        int height = 800;
        init_tga_data(width, height);
        FACE f;
        VERTEX v0, v1, v2, vt0, vt1, vt2;
        float intensity, zbuf[width * height];
        for (int i = width * height; i--; zbuf[i] = INT_MIN);
        Vec3f lightdir = {0, 0, -1};
        for(int i = 0; i < model.fnum; i++) {
                f = model.faces[i];
                v0 = model.vertices[f.v0-1];
                v1 = model.vertices[f.v1-1];
                v2 = model.vertices[f.v2-1];
                vt0 = model.vtexture[f.vt0-1];
                vt1 = model.vtexture[f.vt1-1];
                vt2 = model.vtexture[f.vt2-1];

                VERTEX v2mv0 = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};
                VERTEX v1mv0 = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
                VERTEX v2mxorvm1 = {v2mv0.y*v1mv0.z - v2mv0.z*v1mv0.y,
                                    v2mv0.z*v1mv0.x - v2mv0.x*v1mv0.z,
                                    v2mv0.x*v1mv0.y - v2mv0.y*v1mv0.x};
                float norm = sqrt(v2mxorvm1.x * v2mxorvm1.x + v2mxorvm1.y*v2mxorvm1.y + v2mxorvm1.z*v2mxorvm1.z);
                VERTEX vnorm = {v2mxorvm1.x/norm, v2mxorvm1.y/norm, v2mxorvm1.z/norm};
                intensity = vnorm.x*lightdir.x + vnorm.y*lightdir.y + vnorm.z*lightdir.z;

                v0.x = (v0.x + 1.) * width / 2.;
                v0.y = (v0.y + 1.) * height / 2.;
                v0.z = (v0.z + 1.) * height / 2.;
                v1.x = (v1.x + 1.) * width / 2.;
                v1.y = (v1.y + 1.) * height / 2.;
                v1.z = (v1.z + 1.) * height / 2.;
                v2.x = (v2.x + 1.) * width / 2.;
                v2.y = (v2.y + 1.) * height / 2.;
                v2.z = (v2.z + 1.) * height / 2.;

                if (intensity > 0)
                        triangle(v0, v1, v2, vt0, vt1, vt2, zbuf, model.texture,
                                 intensity);
        }
        write_free_tga("/mnt/c/Users/stasyaner/Desktop/african_head.tga");
        return 0;
}

void free_model() {
        if (model.vnum){
                free(model.vertices);
                model.vnum = 0;
        }
        if (model.fnum) {
                free(model.faces);
                model.fnum = 0;
        }
        if (model.texture.data)
                free(model.texture.data);
}
