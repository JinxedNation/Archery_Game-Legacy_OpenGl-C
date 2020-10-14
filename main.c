#ifdef _WIN32
    #include <gl/freeglut.h>
#elif _WIN64
    #include <gl/freeglut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "load.h"

#define MSECS 10    //millisecond value to loop animation timer
#define MAXROW 200  //height of images
#define MAXCOL 320  //width of images

void acceleration();
void setShootPos();
void quitWindow();
void optionMenu();

typedef unsigned char pixel;

point3 arrowStVel = {0,  0,   -1};
point3 arrowStPos = {0,  3.1,  9};
point3 acc        = {0, -0.98, 0};

Object arrow, n_arrow, bow, n_bow;

pixel p, image_buf[3][MAXROW * MAXCOL];

pixel image[3][MAXROW][MAXCOL];

FILE *inf_handle[3], *outf_handle;

float angleX = 0.0f;
float angleY = 0.0f;
float angle  = 0.0;

float lx = 1.0f;
float ly = 0.0f;
float lz = 0.0f;

float eyex = 0.0f;
float eyey = 3.0f;
float eyez = 10.0f;

float deltaAngle = 0.0f;
float deltaMove  = 0;

float crosshairX[] = {0, 0.01, -0.01};
float crosshairY[] = {3, 3.01, 2.99};

float prevTime    = 0;
float floorHeight = 0;

float chargePosX[] = {-0.64, -0.6};
float chargePosY[] = {2.7, 2.7};

float targetX[] = { -1,  2,  -5,   9,  13.5};
float targetY[] = {  2,  4,   3,   4,   2.6};
float targetZ[] = {-10, -3, -13, -20, -20};
float targetScale = 1.2;

bool arrowState = false, holdDown = false, arrowReady = true, windOn = true, full = true, redisplay = true;

float arrowRotation = 0;

float bowX = 0.21;
float speed = 0.00015;
float hSpeed = 1;

float dx = 0;
float dy = 0;

float lastx = 0;
float lasty = 0;

int charin;
int rows;
int colums;

float windspeed     = 0;
int windDirection   = 0;
char infile[3][10]  = {"chris.raw", "evan.raw", "will.raw"};
char caption[3][32] = {"Christopher Logan", "Evan Chrisp", "William Halling Howard"};

point3 wind = {0,0,0};

float lastChargeY;
float cornerScale = 1;
float houseScale  = 6;
float wallPos     = -75;

point3 crossColour[] = {{1,1,1},{0,0,0},{1.00,0.20,1.00},{0.30,1.00,1.00}, {1, 1, 0.2}};
int chosenCol = 0;

void changeSize(int w, int h)
{
	if (h == 0)

		h = 1;

	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

//Draws a house
void drawHouse()
{
    //Draws the house
    glColor3f(1.00,0.50,0.75);
    glBegin(GL_POLYGON);
        glVertex3f(-10, 0.0, -14.999);
        glVertex3f( -9, 0.0, -14.999);
        glVertex3f( -9, 2.0, -14.999);
        glVertex3f(-10, 2.0, -14.999);
    glEnd();


    glColor3f(0.40,0.80,1.00);
    glBegin(GL_POLYGON);
        glVertex3f(-12.0, 2.1, -14.999);
        glVertex3f(-10.5, 2.1, -14.999);
        glVertex3f(-10.5, 3.6, -14.999);
        glVertex3f(-12.0, 3.6, -14.999);
    glEnd();


    glBegin(GL_POLYGON);
        glVertex3f(-7.0, 2.1, -14.999);
        glVertex3f(-8.5, 2.1, -14.999);
        glVertex3f(-8.5, 3.6, -14.999);
        glVertex3f(-7.0, 3.6, -14.999);
    glEnd();


    //front wall
    glColor3f(1.00,0.47,0.20);
    glBegin(GL_POLYGON);
        glVertex3f(-13, 0.0, -15.0);
        glVertex3f( -6, 0.0, -15.0);
        glVertex3f( -6, 5.0, -15.0);
        glVertex3f(-13, 5.0, -15.0);
    glEnd();


    //back wall
    glBegin(GL_POLYGON);
        glVertex3f(-13, 0.0, -25.0);
        glVertex3f( -6, 0.0, -25.0);
        glVertex3f( -6, 5.0, -25.0);
        glVertex3f(-13, 5.0, -25.0);
    glEnd();

    //left wall
    glBegin(GL_POLYGON);
        glVertex3f(-13, 0.0, -25.0);
        glVertex3f(-13, 0.0, -15.0);
        glVertex3f(-13, 5.0, -15.0);
        glVertex3f(-13, 5.0, -25.0);
    glEnd();

    //right wall
    glBegin(GL_POLYGON);
        glVertex3f(-6, 0.0, -25.0);
        glVertex3f(-6, 0.0, -15.0);
        glVertex3f(-6, 5.0, -15.0);
        glVertex3f(-6, 5.0, -25.0);
    glEnd();

    //left side roof
    glColor3f(0.50,0.16,0.00);
    glBegin(GL_TRIANGLES);
        glVertex3f(-13, 7, -20.0);
        glVertex3f(-13, 5, -25.0);
        glVertex3f(-13, 5, -15.0);
    glEnd();

    //right side
    glBegin(GL_TRIANGLES);
        glVertex3f(-6, 7, -20.0);
        glVertex3f(-6, 5, -25.0);
        glVertex3f(-6, 5, -15.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f( -6, 7, -20);
        glVertex3f(-13, 7, -20);
        glVertex3f(-13, 5, -25);
        glVertex3f( -6, 5, -25);
   glEnd();


    glBegin(GL_POLYGON);
        glVertex3f( -6, 7, -20);
        glVertex3f(-13, 7, -20);
        glVertex3f(-13, 5, -15);
        glVertex3f( -6, 5, -15);
    glEnd();

    glFlush();
}

//Draws a wall
void drawWall()
{
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_POLYGON);
        glVertex3f( 1000,  0.0, -75);
        glVertex3f(-1000,  0.0, -75);
        glVertex3f(-1000, 20.0, -75);
        glVertex3f( 1000, 20.0, -75);
    glEnd();
}

//Draws a line of fences
void drawFence()
{
    //front side of the picket
    for(int i = -100; i < 100; i++)
    {
        glColor3f(1.00,0.47,0.20);
        glPushMatrix();
        glTranslatef(i, 0, 0);


        glBegin(GL_POLYGON);
            glVertex3f(0.5, 0.0, 6);
            glVertex3f(0.0, 0.0, 6);
            glVertex3f(0.0, 2.0, 6);
            glVertex3f(0.5, 2.0, 6);
        glEnd();


        glBegin(GL_POLYGON);
            glVertex3f(1, 1.4, 6);
            glVertex3f(0, 1.4, 6);
            glVertex3f(0, 0.9, 6);
            glVertex3f(1, 0.9, 6);
        glEnd();


        glBegin(GL_TRIANGLES);
            glVertex3f(0.25, 2.3, 6);
            glVertex3f(0, 2, 6);
            glVertex3f(0.5, 2, 6);
        glEnd();
        glPopMatrix();
    }
}


//When you are moved it makes calculations
void computePosition(float deltaMove)
{
    eyex+= deltaMove * hSpeed * 0.1;
    crosshairX[0] += deltaMove * hSpeed * 0.1;
    arrowStPos[0] = crosshairX[0];

    crosshairX[1] += deltaMove * hSpeed * 0.1;
    crosshairX[2] += deltaMove * hSpeed * 0.1;

    chargePosX[0] += deltaMove * hSpeed * 0.1;
    chargePosX[1] += deltaMove * hSpeed * 0.1;

    bowX += deltaMove * lx * 0.1;
}


//When rotations are made calculations are done here
void computeDirection()
{
    if(angle > 1.65)
    {
        angle = 1.65;
        crosshairY[0] = 3.1;
        crosshairY[1] = 3.11;
        crosshairY[2] = 3.09;


        if(holdDown)
        {
            chargePosY[0] = 2.8f;
            chargePosY[1] = lastChargeY;
        }
    }
    else
        if(angle < -1.65)
        {
            angle = -1.65;
            crosshairY[0] = 2.9;
            crosshairY[1] = 2.91;
            crosshairY[2] = 2.89;

            if(holdDown)
            {
                chargePosY[0] = 2.6f;
                chargePosY[1] = lastChargeY;
            }
        }

    arrowStPos[1] = crosshairY[0];
    ly = sin(angle);
    lz = -cos(angle);
}

//draws the crosshair in the middle of the screen
void drawCrosshair()
{
    glColor3f(crossColour[chosenCol][0], crossColour[chosenCol][1], crossColour[chosenCol][2]);
    glLineWidth(1);
    glPushMatrix();


    glBegin(GL_LINES);
            glVertex3f(crosshairX[0], crosshairY[1], 9.0);
            glVertex3f(crosshairX[0], crosshairY[2], 9.0);
        glEnd();
    glPopMatrix();


    glBegin(GL_LINES);
        glVertex3f(crosshairX[1], crosshairY[0], 9.0);
        glVertex3f(crosshairX[2], crosshairY[0], 9.0);
    glEnd();
}

//creates the colours and layout of the targets
void drawTargets()
{
    GLUquadricObj *blue = gluNewQuadric();
    glColor3f(0, 0, 1);
    gluCylinder(blue, 1.2, .8, 0, 50, 1);


    GLUquadricObj *red = gluNewQuadric();
    glColor3f(1, 0, 0);
    gluCylinder(red, .8, 0.3, 0, 50, 1);


    GLUquadricObj *yellow = gluNewQuadric();
    glColor3f(1, 1, 0);
    gluCylinder(yellow, 0.3, 0, 0, 50, 1);
}

//draws the various objects in the scene
void render()
{
    if(deltaMove)

        computePosition(deltaMove);


    if(angle)

        computeDirection();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    gluLookAt(eyex, eyey, eyez, eyex, eyey + ly, 0, 0.0f, 1.0f, 0.0f);
    glClearColor(0.1, 0.7, 1, 1);


    //ground
    glColor3f(0.0f, 0.7f, 0.0f);
    glBegin(GL_QUADS);
		glVertex3f(-1000.0f, floorHeight, -1000.0f);
		glVertex3f(-1000.0f, floorHeight,  1000.0f);
		glVertex3f( 1000.0f, floorHeight,  1000.0f);
		glVertex3f( 1000.0f, floorHeight, -1000.0f);
	glEnd();

	drawWall();
    drawHouse();
    drawFence();


    //targets
	for(int i = 0; i < 5; i++)
    {
        glPushMatrix();
            glTranslatef(targetX[i], targetY[i], targetZ[i]);
            glScalef(targetScale, targetScale, targetScale);
            drawTargets();
        glPopMatrix();
    }

    //powerbar
    acceleration();


    //crosshair
    drawCrosshair();


	//arrow
    glPushMatrix();
        glColor3f(0.6, 0.3, 0.01);
        glTranslatef(n_arrow.pos[0][0], n_arrow.pos[0][1], n_arrow.pos[0][2]);
        glScalef(n_arrow.scale, n_arrow.scale, n_arrow.scale);
        glRotatef(-90, 0, 1, 0);
        glRotatef(arrowRotation, 0, 0, 1);
        drawObject(&n_arrow);
    glPopMatrix();


    //bow
    glPushMatrix();
        glColor3f(0.6, 0.3, 0.01);
        glTranslatef(bowX, crosshairY[0]+0.02, 9);
        glScalef(n_bow.scale, n_bow.scale, n_bow.scale);
        glRotatef(90, 1, 0, 0);
        glRotatef(45, 0, 0, 1);
        drawObject(&n_bow);
    glPopMatrix();
    glutSwapBuffers();
}

//called when user exits the program
//reads the group member images into an array
void quit()
{
    for(int i = 0; i < 3; i++)
    {
        if((inf_handle[i] = fopen(infile[i], "rb")) == NULL)
        {
             charin = getchar();
             exit(1);
        }


        for (rows = 0;  rows < MAXROW; rows++ )
        {
            for (colums = 0;  colums < MAXCOL; colums++)
            {
                if((charin = fgetc(inf_handle[i])) == EOF)
                {
                    charin = getchar();
                    exit(1);
                }

                image[i][rows][colums] = charin;
             }
        }

        fclose(inf_handle[i]);
    }

    quitWindow();
}

//displays the exit window with the group member names and photos
void display()
{
    int	offset;
    int rows;
    int colums;


    for(int i = 0; i < 3; i++)
    {
        offset = 0;

        for (rows = MAXROW - 1; rows >= 0; rows-- )
        {
            for(colums = 0; colums < MAXCOL; colums++)
            {
                image_buf[i][MAXCOL * offset + colums] =  image[i][rows][colums];
            }
            offset++;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,760.0, 0.0,280.0);
    glMatrixMode(GL_MODELVIEW);
    glColor3f(0.0, 0.0, 0.0);

    //Chris
    glRasterPos2i(100, 150);
    glDrawPixels(MAXCOL, MAXROW, GL_LUMINANCE, GL_UNSIGNED_BYTE, image_buf[0]);
    glRasterPos2i(150, 120);


    for (int i = 0; i < sizeof(caption[0]); i++)

        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caption[0][i]);


    //Evan
    glRasterPos2i(300, 150);
    glDrawPixels(MAXCOL, MAXROW, GL_LUMINANCE, GL_UNSIGNED_BYTE, image_buf[1]);
    glRasterPos2i(360, 120);


    for (int i = 0; i < sizeof(caption[1]); i++)

        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caption[1][i]);


    //Will
    glRasterPos2i(500, 150);
    glDrawPixels(MAXCOL, MAXROW, GL_LUMINANCE, GL_UNSIGNED_BYTE, image_buf[2]);
    glRasterPos2i(540, 120);


    for (int i = 0; i < sizeof(caption[2]); i++)

        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, caption[2][i]);


    glFlush();
    free(n_arrow.nums);
    free(arrow.nums);
    free(bow.nums);
    free(n_bow.nums);
}

//creates the window at program exit
void quitWindow()
{
    glutDestroyWindow(glutGetWindow());
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1280, 720);
    glutCreateWindow("Arrow Shooting Game");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
}

void crossHairMenu(int colums)
{
    switch(colums)
    {
        case 0:
            chosenCol = colums;
            break;

        case 1:
            chosenCol = colums;
            break;

        case 2:
            chosenCol = colums;
            break;

        case 3:
            chosenCol = colums;
            break;

        case 4:
            chosenCol = colums;
            break;
    }
}

//submenu for user selecting wind speed
void subMenuHandler(int choice)
{
    switch(choice)
    {
        case 0:
            windspeed = 0;
            break;

        case 1:
            windspeed = 0.50;
            break;

        case 2:
            windspeed = 1.00;
            break;

        case 3:
            windspeed = 1.5;
            break;

        case 4:
            windspeed = 2;
            break;

        case 5:
            windspeed = 2.5;
            break;
    }
}

//submenu for user selecting wind direction
void subMenuHandler2(int choice)
{
    switch(choice)
    {
        case 0:
            windDirection = 0;
            wind[0] = 0;
            wind[1] = 0;
            wind[2] = 0;
            break;

        case 1:
            windDirection = 1;
            wind[0] = -1;
            wind[1] =  0;
            wind[2] =  0;
            break;

        case 2:
            windDirection = 2;
            wind[0] = 1;
            wind[1] = 0;
            wind[2] = 0;
            break;

        case 3:
            windDirection = 3;
            wind[0] =  0;
            wind[1] =  0.005;
            wind[2] = -1;
            break;

        case 4:
            windDirection = 4;
            wind[0] = -0.75;
            wind[1] =  0;
            wind[2] = -0.5;
            break;

        case 5:
            windDirection = 5;
            wind[0] =  0.75;
            wind[1] =  0;
            wind[2] = -0.5;
            break;
    }
}

//handles the selected menu option
void mainMenuHandler(int choice)
{
    switch(choice)
    {
        case 1:
            if(full)
            {
                glutPositionWindow(200, 200);
                glutReshapeWindow(1280, 720);
                full = false; //window is windowed
            }
            else
            {
                glutFullScreen();
                full = true; //window is fullscreen
            }
            break;

        case 2:
            redisplay = false; //prevent trying to redisplay non-existent window
            quit();
            break;
    }
}

//displays an option when user right-clicks
//allows changing of wind speed and direction, window mode, or exit program
void optionMenu()
{
    int submenu = glutCreateMenu(subMenuHandler);
	glutAddMenuEntry("  None     ", 0);
	glutAddMenuEntry("  Speed 1  ", 1);
	glutAddMenuEntry("  Speed 2  ", 2);
	glutAddMenuEntry("  Speed 3  ", 3);
	glutAddMenuEntry("  Speed 4  ", 4);
	glutAddMenuEntry("  Speed 5  ", 5);

	int submenu2 = glutCreateMenu(subMenuHandler2);
	glutAddMenuEntry("  None       ", 0);
	glutAddMenuEntry("  West       ", 1);
	glutAddMenuEntry("  East       ", 2);
	glutAddMenuEntry("  North      ", 3);
	glutAddMenuEntry("  North-West ", 4);
	glutAddMenuEntry("  North-East ", 5);

    int submenu3 = glutCreateMenu(crossHairMenu);
	glutAddMenuEntry("  White  ", 0);
	glutAddMenuEntry("  Black  ", 1);
	glutAddMenuEntry("  Pink   ", 2);
	glutAddMenuEntry("  Blue   ", 3);
	glutAddMenuEntry("  Yellow ", 4);

	glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Wind Speed       ", submenu);
	glutAddSubMenu("Wind Direction   ", submenu2);
	glutAddSubMenu("Crosshair Colour ", submenu3);
	glutAddMenuEntry("Window Size    ", 1);
	glutAddMenuEntry("Exit           ", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//function to process various key presses
void pressKey(unsigned char key, int x, int y)
{
    if(key == 'a' || key == 'A') //start moving left
    {
        deltaMove = 0;

        if(eyex > -10)
        {
            deltaMove = -0.5f;
        }
    }
    else

        if(key == 'd' || key == 'D') //start moving right
        {
            deltaMove = 0;

            if(eyex < 10)
            {
                deltaMove = 0.5f;
            }
        }
        else

            if(key == 'q' || key == 'Q' || key == 27) //exit program
            {
                quit();
            }
            else

                if(key == ' ')
                {
                    if(arrowReady)
                    {
                        holdDown = true;
                    }
                }

    if(key == 'f' || key == 'F') //toggle fullscreen/windowed mode
    {
        if(full)
        {
            glutPositionWindow(200, 200);
            glutReshapeWindow(1280, 720);
            full = false;
        }

        else
        {
            glutFullScreen();
            full = true;
        }
    }
}

//handles the user moving their camera view up or down
//also moves the crosshair and position of arrow power bar
void mouseMove(int xx, int yy)
{
    int i;
    dx = xx - lastx;
    dy = yy - lasty;

    if(dy > 0)
    {
         angle += -0.01f;

         chargePosY[0] += -0.0006f;
         chargePosY[1] += -0.0006f;

         for(i = 0; i < 3; i++)
         {
            crosshairY[i] += -0.0006f;
         }
    }
    else
        if(dy < 0)
        {
            angle += 0.01f;

            chargePosY[0] += 0.0006f;
            chargePosY[1] += 0.0006f;

            for(i = 0; i < 3; i++)
            {
                crosshairY[i] += 0.0006f;
            }
        }

    lastx = xx;
    lasty = yy;
}

//processes various key releases
void releaseKey(unsigned char key, int x, int y)
{
    if(key == 'a' || key == 'A') //stop moving left
    {
        deltaMove = 0;
    }
    else

        if(key == 'd' || key == 'D') //stop moving right
        {
            deltaMove = 0;
        }
        else

            if(key == ' ')
            {
                if(arrowReady)
                {
                    setShootPos();
                    arrowState = true;
                    holdDown = false;
                    arrowReady = false;
                }
            }
}

//handles mouse click functions
void mouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_RIGHT_BUTTON) //open option menu
    {
        optionMenu();
    }
}

//loads in the OFF files into the program
void loadModels()
{
    readOFFFile("arrow.off", &arrow); //arrow model

        n_arrow.nvert    = arrow.nvert;
        n_arrow.nfaces   = arrow.nfaces;

        n_arrow.scale    = 1;
        n_arrow.location = (point3*) malloc((n_arrow.nvert)*sizeof(point3));

        n_arrow.nums     = (Face*) malloc((n_arrow.nfaces)*sizeof(Face));
        normalize3DModel(&arrow, &n_arrow);
        n_arrow.rad     *= n_arrow.scale;

        n_arrow.pos[0][0] = -10;
        n_arrow.pos[0][1] = -10;
        n_arrow.pos[0][2] = -10;

        n_arrow.pos[1][0] = -10;
        n_arrow.pos[1][1] = -10;
        n_arrow.pos[1][2] = -10;


    readOFFFile("bow.off", &bow); //bow model

        n_bow.nvert  = bow.nvert;
        n_bow.nfaces = bow.nfaces;
        n_bow.scale  = 0.5;

        n_bow.location = (point3*) malloc((n_bow.nvert)*sizeof(point3));
        n_bow.nums     = (Face*)   malloc((n_bow.nfaces)*sizeof(Face));
        normalize3DModel(&bow, &n_bow);
        n_bow.rad    *= n_bow.scale;
}

void setShootPos()
{
    n_arrow.pos[0][0] = arrowStPos[0];
    n_arrow.pos[0][1] = arrowStPos[1];
    n_arrow.pos[0][2] = arrowStPos[2];

    n_arrow.pos[1][0] = arrowStPos[0];
    n_arrow.pos[1][1] = arrowStPos[1];
    n_arrow.pos[1][2] = arrowStPos[2];

    n_arrow.vel[0][0] = arrowStVel[0];
    n_arrow.vel[0][1] = arrowStVel[1];
    n_arrow.vel[0][2] = arrowStVel[2];

    n_arrow.vel[1][0] = arrowStVel[0];
    n_arrow.vel[1][1] = arrowStVel[1];
    n_arrow.vel[1][2] = arrowStVel[2];
}

//only called once arrow collides with target or ground
void hasCollided()
{
    arrowReady = true;      //allow arrow to be shot again
    arrowState = false;
    acc[2] = 0;
    chargePosY[1] = chargePosY[0]; //reset arrow speed charge
    n_arrow.vel[0][0] = 0;
    n_arrow.vel[0][1] = 0;
    n_arrow.vel[0][2] = 0;
}

//Checks if the arrow is hitting the house
void houseCollision()
{
    bool xCheck = (n_arrow.pos[0][0] <=  -6 && n_arrow.pos[0][0] >= -13);
    bool yCheck = (n_arrow.pos[0][1] <=   7 && n_arrow.pos[0][1] >=   0);
    bool zCheck = (n_arrow.pos[0][2] <= -15 && n_arrow.pos[0][2] >= -25);


    if(xCheck && yCheck && zCheck)
    {
        hasCollided();
    }
}

//Checks if the arrow is hitting the wall
void wallCollision()
{
    if(n_arrow.pos[0][2] <= wallPos)

        hasCollided();
}

//checks if an arrow is colliding with a target
void arrowCollision()
{
    GLfloat distToTarget[3];

    for(int i = 0; i < 5; i++)
    {
        GLfloat dX = n_arrow.pos[0][0] - targetX[i];
        GLfloat dY = n_arrow.pos[0][1] - targetY[i];
        GLfloat dZ = n_arrow.pos[0][2] - targetZ[i];


        distToTarget[i] = sqrt(dX*dX + dY*dY + dZ*dZ);


        if(distToTarget[i] <= targetScale * targetScale) //arrow has collided if true
        {
            hasCollided();
        }
    }
}


//checks if an arrow is colliding with the ground
void floorCollision()
{
    if(n_arrow.pos[0][1] <= floorHeight)    //arrow has collided with ground if true
    {
        hasCollided();
    }
}

//displays the power bar of the arrows speed as the user holds down spacebar
void acceleration()
{
    if(holdDown)
    {
        glColor3f(1.0, 0.0, 0.0);

        if(acc[2] > -20.0)
        {
            glColor3f(0.0, 0.0, 1.0);
            acc[2] -= 0.1;
            chargePosY[1] += 0.0005;
        }

        glPushMatrix();
            glTranslatef(0, 0, 9);
            glRectf(chargePosX[0], chargePosY[0], chargePosX[1], chargePosY[1]);
        glPopMatrix();


        lastChargeY = chargePosY[1];
    }
}

//animates and calculates the various physics of the arrow as it flies through the sky
void animateArrow()
{
    glutTimerFunc(MSECS, animateArrow, 0);

    float currTime           = glutGet(GLUT_ELAPSED_TIME) / 1000.0 ;
    float timeSincePrevFrame = currTime - prevTime;

    if(arrowState)
    {
        n_arrow.pos[0][0] = n_arrow.pos[1][0] + n_arrow.vel[0][0] * timeSincePrevFrame + acc[0] / 2 * (timeSincePrevFrame*timeSincePrevFrame);
        n_arrow.pos[0][1] = n_arrow.pos[1][1] + n_arrow.vel[0][1] * timeSincePrevFrame + acc[1] / 2 * (timeSincePrevFrame*timeSincePrevFrame);
        n_arrow.pos[0][2] = n_arrow.pos[1][2] + n_arrow.vel[0][2] * timeSincePrevFrame + acc[2] / 2 * (timeSincePrevFrame*timeSincePrevFrame);

        n_arrow.vel[0][0] = n_arrow.vel[1][0] + acc[0] * timeSincePrevFrame + wind[0] * windspeed * timeSincePrevFrame;
        n_arrow.vel[0][1] = n_arrow.vel[1][1] + acc[1] * timeSincePrevFrame + ly      * timeSincePrevFrame;
        n_arrow.vel[0][2] = n_arrow.vel[1][2] + acc[2] * timeSincePrevFrame + wind[2] * windspeed * timeSincePrevFrame;;

        arrowCollision();   //check if colliding with target
        floorCollision();   //check if colliding with ground
        houseCollision();   //check if colliding with house
        wallCollision();    //check if colliding with wall

        n_arrow.pos[1][0] = n_arrow.pos[0][0];
        n_arrow.pos[1][1] = n_arrow.pos[0][1];
        n_arrow.pos[1][2] = n_arrow.pos[0][2];

        n_arrow.vel[1][0] = n_arrow.vel[0][0];
        n_arrow.vel[1][1] = n_arrow.vel[0][1];
        n_arrow.vel[1][2] = n_arrow.vel[0][2];
    }


    prevTime = currTime;

    if(redisplay)   //dont redisplay window if program exits

        glutPostRedisplay();
}


void init()
{
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble fov     =  80;
    GLdouble aspect  =   1;
    GLdouble nearVal =   0.1;
    GLdouble farVal = 1000;
    gluPerspective(fov, aspect, nearVal, farVal);

    loadModels();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);                                  //initiate the glut session
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);  //depth buffer, double buffered and RGBA
    glutCreateWindow("Arrow Shooting Game");                //the window title
    glutFullScreen();                                       //display the game in fullscreen by default
    init();

    glutDisplayFunc(render);                                //set display callback for this window
    glutReshapeFunc(changeSize);                            //set window reshape callback
    glutTimerFunc(MSECS, animateArrow, 0);                  //start animation loop

    glutKeyboardFunc(pressKey);                             //key press callback
    glutKeyboardUpFunc(releaseKey);                         //key release callback
    glutMouseFunc(mouseClick);                              //mouse click callback
    glutPassiveMotionFunc(mouseMove);                       //mouse movement callback
    glutSetCursor(GLUT_CURSOR_NONE);                        //removes cursor while in the window

    glEnable(GL_DEPTH_TEST);                                //update depth buffer
    glutMainLoop();                                         //keep calling display functions


	return 0;
}