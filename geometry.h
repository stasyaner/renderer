#pragma once

#include <math.h>

#define vcproduct(v0, v1, resv) do {\
        resv.x = v0.y*v1.z - v0.z*v1.y;\
        resv.y = v0.z*v1.x - v0.x*v1.z;\
        resv.z = v0.x*v1.y - v0.y*v1.x;\
} while(0)
#define vsprod(v0, v1, resv) do {\
        resv = v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;\
} while(0)
#define vsub(v0, v1, resv) do {\
        resv.x = v1.x - v0.x;\
        resv.y = v1.y - v0.y;\
        resv.z = v1.z - v0.z;\
} while(0)
#define vabs(v, absres) do {absres=sqrt(v.x*v.x + v.y*v.y + v.z*v.z);} while(0)
#define vnormalize(v, vnormres) do {\
        double vabst;\
        vabs(v, vabst);\
        vnormres.x = v.x / vabst;\
        vnormres.y = v.y / vabst;\
        vnormres.z = v.z / vabst;\
} while(0)

typedef struct {
        float x, y, z;
} Vec3f;

typedef struct {
        int x, y, z;
} Vec3i;

typedef struct {
        float x, y;
} Vec2f;

typedef struct {
        int x, y;
} Vec2i;
