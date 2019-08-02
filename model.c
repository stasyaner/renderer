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
        int depth = 255;
        init_tga_data(width, height);
        FACE f;
        VERTEX v0, v1, v2, vt0, vt1, vt2;
        Vec3f ab, ac, tricprod, trinorm;
        float intensity, zbuf[width * height];
        for (int i = width * height; i--; zbuf[i] = INT_MIN);
        Vec3f lightdir = {0, 0, -1};
        Vec3f camera = {0, 0, 3};
        float mv0[4], mv1[4], mv2[4], vp[4][4], mres[4][4];
        float projection[4][4] = {{1, 0,           0, 0},
                                  {0, 1,           0, 0},
                                  {0, 0,           1, 0},
                                  {0, 0, -1/camera.z, 1}};
        viewport(width/8, height/8, width*3/4, height*3/4, depth, vp);
        mmulm(vp, projection, mres);
        for(int i = 0; i < model.fnum; i++) {
                f = model.faces[i];
                v0 = model.vertices[f.v0-1];
                v1 = model.vertices[f.v1-1];
                v2 = model.vertices[f.v2-1];
                vt0 = model.vtexture[f.vt0-1];
                vt1 = model.vtexture[f.vt1-1];
                vt2 = model.vtexture[f.vt2-1];

                vsub(v2, v0, ab);
                vsub(v1, v0, ac);
                vcproduct(ab, ac, tricprod);
                vnormalize(tricprod, trinorm);
                vsprod(trinorm, lightdir, intensity);

                mmulv(mres, v0, mv0);
                mmulv(mres, v1, mv1);
                mmulv(mres, v2, mv2);
                m2v(mv0, v0);
                m2v(mv1, v1);
                m2v(mv2, v2);

                vt0.x = vt0.x * model.texture.header.width;
                vt0.y = vt0.y * model.texture.header.height;
                vt1.x = vt1.x * model.texture.header.width;
                vt1.y = vt1.y * model.texture.header.height;
                vt2.x = vt2.x * model.texture.header.width;
                vt2.y = vt2.y * model.texture.header.height;

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
