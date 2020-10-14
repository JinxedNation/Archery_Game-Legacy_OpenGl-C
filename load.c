#include "load.h"

GLfloat vectorNorm(point3 p)
{
	GLfloat n = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2] );

	return n;
}

GLfloat max(GLfloat a, GLfloat b)
{
	if (a > b)

		return a;

	else
    {
		return b;
	}
}

void readOFFFile(char *fileName, Object *object)
{
    int i, x, j;
    char off[25];
    FILE *file = fopen(fileName, "r");//Opens the file

    if(file == NULL)
    {
        printf("Error! Something went wrong with a file! Exiting the program!\n");
        exit(0);
    }

    fscanf(file,"%s %d %d %d", off, &object->nvert, &object->nfaces, &x);

    object->location = malloc(object->nvert * sizeof(point3));//Allocated memory the size of nvert * 3 floats, which should be 12 as floats have 4 bytes each
    object->nums    = malloc(object->nfaces * sizeof(Face));//Same as above but with ints

    for(i = 0; i < object->nvert; i ++)
    {
        fscanf(file,"%f %f %f", &object->location[i][0], &object->location[i][1], &object->location[i][2]);//Puts values in a vert array with x, y, z
    }

    for(i = 0; i < object->nfaces; i++)
    {
        fscanf(file,"%d %d %d %d", &j, &object->nums[i][0], &object->nums[i][1], &object->nums[i][2]);//Puts values in a nums array with num1, num2, num3
    }
    fclose(file);
}

void computeBoundingSphere(const  Object* obj, point3 centre, GLfloat* radius)
{
	// 1. Computing the centre
	centre[0] = 0.f;
	centre[1] = 0.f;
	centre[2] = 0.f;
	int i, j;

	for (i=0; i<obj->nvert; i++){
		for (j=0; j<3; j++)
			centre[j] = centre[j] + (obj->location)[i][j];
	}

	for (j = 0; j < 3; j++)

		centre[j] = centre[j] / obj->nvert;

	// 2. Computing the radius of the bounding sphere
	GLfloat r = 0;		// a very large number

	for (i = 0; i < obj->nvert; i++)
    {
		point3 v;

		for (j = 0; j < 3; j++)

			v[j] = (obj->location)[i][j] - centre[j];


		// norm of v
		r = max(r, vectorNorm(v));
	}

	*radius = r;
}

void normalize3DModel(const Object* obj, Object* obj_normalized)
{
	point3 centre;
	GLfloat radius;
	int i;

	// 1. Compute the bounding sphre of the 3D model
	computeBoundingSphere(obj, centre, &radius);


	// 2. Translate the object so that its centre is at (0, 0, 0)
	for (i = 0; i < obj->nvert; i++)
    {
		obj_normalized->location[i][0] = obj->location[i][0] - centre[0];
		obj_normalized->location[i][1] = obj->location[i][1] - centre[1];
		obj_normalized->location[i][2] = obj->location[i][2] - centre[2];
	}


	// 3. Scale all the vertices of the object so that it fit inside the unit sphere
	for (i = 0; i < obj->nvert; i++)
    {
		obj_normalized->location[i][0] = obj_normalized->location[i][0] / radius;
		obj_normalized->location[i][1] = obj_normalized->location[i][1] / radius;
		obj_normalized->location[i][2] = obj_normalized->location[i][2] / radius;
	}

	// 4. Just copy the faces
	for (i = 0; i < obj->nfaces; i++)
    {
		obj_normalized->nums[i][0] = obj->nums[i][0];
		obj_normalized->nums[i][1] = obj->nums[i][1];
		obj_normalized->nums[i][2] = obj->nums[i][2];
	}

	computeBoundingSphere(obj_normalized, centre, &radius);
	obj_normalized->rad = radius;
}

//adds light to the objects
void addLight()
{
    GLfloat light_position[] = {1, 5, 10, 0.5};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
    glEnable(GL_COLOR_MATERIAL);
}

void drawObject(Object *obj)
{
    addLight();
    for(int i = 0; i < obj->nfaces; i++)
    {
        glBegin(GL_TRIANGLES);
            Face f;
            f[0] = obj->nums[i][0];
            f[1] = obj->nums[i][1];
            f[2] = obj->nums[i][2];

            glVertex3fv(obj->location[f[0]]);
            glVertex3fv(obj->location[f[1]]);
            glVertex3fv(obj->location[f[2]]);
        glEnd();
    }
}
