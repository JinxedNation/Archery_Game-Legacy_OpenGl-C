#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stdbool.h>

typedef GLfloat point2[2];
typedef GLfloat point3[3];
typedef int Face[3];

typedef struct
{
    float scale;
    float rad;
    point3* location;
    Face* nums;
    int nvert;
    int nfaces;
    point3 vel[2];//Not all objects will use these, only the arrows
    point3 pos[2];//Not all objects will use these, only the arrows
} Object;

GLfloat vectorNorm(point3 p);

GLfloat max(GLfloat a, GLfloat b);


void readOFFFile(char *fileName, Object *object);

void computeBoundingSphere(const  Object* obj, point3 centre, GLfloat* radius);

void normalize3DModel(const Object* obj, Object* obj_normalized);

void addLight();

void drawObject(Object *obj);
