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
#define vsub(v1, v0, resv) do {\
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
#define mmulv(m1, v, mres) do {\
        float t[4] = {v.x, v.y, v.z, 1};\
        for (int i = 0; i < 4; i++) {\
                mres[i] = 0;\
                for (int j = 0; j < 4; j++)\
                        mres[i] += m1[i][j] * t[j];\
        }\
} while(0)
#define mmulm(m1, m2, mres) do {\
        for (int i = 0; i < 4; i++)\
                for (int j = 0; j < 4; j++) {\
                        mres[i][j] = 0;\
                        for (int k = 0; k < 4; k++)\
                                mres[i][j] += m1[i][k] * m2[k][j];\
                }\
} while(0)
#define m2v(m, v) do {\
        v.x = m[0] / m[3];\
        v.y = m[1] / m[3];\
        v.z = m[2] / m[3];\
} while(0)
#define viewport(x, y, w, h, d, m) do {\
        m[0][0]= w/2.f; m[0][1]=     0; m[0][2]=     0; m[0][3]= x+w/2.f;\
        m[1][0]=     0; m[1][1]= h/2.f; m[1][2]=     0; m[1][3]= y+h/2.f;\
        m[2][0]=     0; m[2][1]=     0; m[2][2]= d/2.f; m[2][3]=   d/2.f;\
        m[3][0]=     0; m[3][1]=     0; m[3][2]=     0; m[3][3]=       1;\
} while(0)
#define lookat(e, c, u, r) do {\
        Vec3f x, y, z;\
        vsub(e, c, z); vnormalize(z, z);\
        vcproduct(u, z, x); vnormalize(x, x);\
        vcproduct(z, x, y); vnormalize(y, y);\
        r[0][0]= x.x; r[0][1]= x.y; r[0][2]= x.z; r[1][3]= -c.x;\
        r[1][0]= y.x; r[1][1]= y.y; r[1][2]= y.z; r[1][3]= -c.y;\
        r[2][0]= z.x; r[2][1]= z.y; r[2][2]= z.z; r[2][3]= -c.z;\
        r[3][0]=   0; r[3][1]=   0; r[3][2]=   0; r[3][3]=    1;\
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
