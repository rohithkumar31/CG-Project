/* Particles in a box-shaped container */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/glut.h>

#define max_no_of_particles 1000
#define initial_no_of_particles 20
#define initial_particle_size 5.0
#define initial_velocity 1.0

typedef int bool;
#define TRUE 1
#define FALSE 0

void myDisplay();
void myIdle();
void myReshape(int, int);
void main_menu(int);
void collision(int);
void myinit();


/* global declaration of the no. of particles */

int num_particles;

/* particle structure */

typedef struct particle
{
     int color;
     float position[3];
     float velocity[3];
     float mass;
};
struct particle particles[max_no_of_particles];

/* initial state of particle system */
int k;
int present_time;
int last_time;
int num_particles =initial_no_of_particles;
float point_size = initial_particle_size;
float speed = initial_velocity;
//bool gravity = FALSE; /* gravity off */
bool elastic = FALSE; /* restitution off */
//bool repulsion = FALSE; /* repulsion off */
float coef = 1.0; /* perfectly elastic collisions */
float d2[max_no_of_particles][max_no_of_particles]; /* array for interparticle distances */





GLsizei wh = 500, ww = 500; /* initial window size */

GLfloat colors[8][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0}};


void drawstring(float x,float y,float z,char *string)
{
	 char *c;
	 glRasterPos3f(x,y,z);
	 for(c=string;*c!='\0';c++)
	 {
		 glColor3f(0,0,0);
		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
	 }
}


void title()          //welcome screen
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
	drawstring(1.2,2.4,0.0,"B.M.S Institute Of Technology & Mgmt.");
	glColor3f(0.0,0.0,0.0);
	drawstring(0.8,1.8,0.0,"Department Of Computer Science And Engineering");
	glColor3f(0.0,0.0,1.0);
	drawstring(1.0,1.7,0.1,"Graphical Implementation of Particle Movement");
	glColor3f(0.0,0.0,1);
	drawstring(-2.0,-0.6,0.0,"Made By :-");
	glColor3f(0.0,0.0,1);
	drawstring(3.5,2.0,0.0,"Guides :-");
	glColor3f(0.0,0.0,0.0);
	drawstring(-2.0,-0.8,0.0,"Rahul R.K (1BY12CS048) ");
	glColor3f(0.0,0.0,0.0);
	drawstring(-1.99,-1.0,0.0,"Rohith Kumar P (1BY12CS053) ");
	glColor3f(0.0,0.0,0.0);
	drawstring(3.5,1.8,0.0,"Ms. Ambika G.N.");
	glColor3f(0.0,0.0,0.0);
	drawstring(3.5,1.6,0.0,"Mrs. Mari Kirthima A.");
	glFlush();
	glutSwapBuffers();
}

/* rehaping routine called whenever window is resized or moved */
void myReshape(int w, int h)
{

/* adjust clipping box */

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-2.0, 2.0, -2.0, 2.0, -4.0, 4.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(1.5,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

/* adjust viewport and  clear */

        if(w<h) glViewport(0,0,w,w);
        else glViewport(0,0,h,h);


/* set global size for use by drawing routine */

        ww = w;
        wh = h;
}

void myinit()
{
        int  i, j;

 /* set up particles with random locations and velocities */

        for(i=0; i<num_particles; i++)
        {
            particles[i].mass = 1.0;
            particles[i].color = i%8;
            for(j=0; j<3; j++)
            {
                particles[i].position[j] = 2.0*((float) rand()/RAND_MAX)-1.0;
                particles[i].velocity[j] = speed*2.0*((float) rand()/RAND_MAX)-1.0;
            }
        }
        glPointSize(point_size);


/* set clear color of background */

        glClearColor(0.9, 0.7, 8.0, 0.0);
}

void myIdle(void)
{
    int i, j;
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001*(present_time -  last_time);
    for(i=0; i<num_particles; i++)
    {
       for(j=0; j<3; j++)
       {
           particles[i].position[j]+=dt*particles[i].velocity[j];    //position=time * speed
           //particles[i].velocity[j]+=dt*forces(i,j)/particles[i].mass;  //velocity=time * (forece/mass)
       }
        collision(i);
    }
    last_time = present_time;
    glutPostRedisplay();
}


void collision(int n)

/* tests for collisions against cube and reflect particles if necessary */

{
     int i;
     for (i=0; i<3; i++)
     {
           if(particles[n].position[i]>=1.0)
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = 1.0-coef*(particles[n].position[i]-1.0);
           }
           if(particles[n].position[i]<=-1.0)
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = -1.0-coef*(particles[n].position[i]+1.0);
           }
     }
}

void main_menu(int index)
{
   switch(index)
   {
        case(0):{
            glutDisplayFunc(myDisplay);
            myinit();
            break;
        }

      case(1):
	{
                num_particles = 2*num_particles;
                myinit();
                break;
     	}
	case(2):
	{
                num_particles = num_particles/2;
                myinit();
		break;
	}
	case(3):
	{
                speed = 2.0*speed;
                myinit();
		break;
	}
        case(4):
        {
                speed = speed/2.0;
                myinit();
                break;
        }
        case(5):
        {
                point_size = 2.0*point_size;
                myinit();
                break;
        }
        case(6):
        {
                point_size = point_size/2.0;
                if(point_size<1.0) point_size = 1.0;
                myinit();
                break;
        }

	case(7):
	{
		exit(0);
		break;
	}
   }
}

void select1()
{
int i;
if(k == 1)
{
    glutWireCube(2.2);
    glBegin(GL_POINTS); /* render all particles */
    for(i=0; i<num_particles; i++)
    {
       glColor3fv(colors[particles[i].color]);
       glVertex3fv(particles[i].position);
    }
    glEnd();
}
else if(k == 2)
{
    glutWireDodecahedron();
    glBegin(GL_POINTS); /* render all particles */
    for(i=0; i<num_particles; i++)
    {
       glColor3fv(colors[particles[i].color]);
       glVertex3fv(particles[i].position);
    }
    glEnd();
}
else if(k == 3)
{    glutWireTeapot(1.9);
    glBegin(GL_POINTS); /* render all particles */
    for(i=0; i<num_particles; i++)
    {
       glColor3fv(colors[particles[i].color]);
       glVertex3fv(particles[i].position);
    }
    glEnd();
}
}


void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,0.0);
    glColor3f(0.0,0.0,0.0);
    drawstring(3.5,3.7,0.0,"Menu");
    glColor3f(0.0,0.0,0.0);
    drawstring(3.5,3.2,0.0,"Press C for Cube");
    glColor3f(0.0,0.0,0.0);
    drawstring(3.5,3.0,0.0,"Press D for Dodecahedron");
    glColor3f(0.0,0.0,0.0);
    drawstring(3.5,2.8,0.0,"Press T for Teapot");
    select1();
      /* outline of box */
   //glutWireDodecahedron();
   //glutWireTeapot(1.8);
    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
    if(key == 'C') k = 1;
    if(key == 'D') k = 2;
    if(key == 'T') k = 3;
   // myDisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1350, 700);
    glutCreateWindow("particle system");
     //glutDisplayFunc(title);
     glutDisplayFunc(title);
    glutKeyboardFunc(keys);
//    glutMouseFunc(mouse);
    myinit();

    glutCreateMenu(main_menu);
    glutAddMenuEntry("Simulation Window", 0);
    glutAddMenuEntry("more particles", 1);
    glutAddMenuEntry("fewer particles", 2);
    glutAddMenuEntry("faster", 3);
    glutAddMenuEntry("slower", 4);
    glutAddMenuEntry("larger particles", 5);
    glutAddMenuEntry("smaller particles", 6);

    glutAddMenuEntry("quit",7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);



    glutIdleFunc(myIdle);
    glutReshapeFunc (myReshape);
    glutMainLoop();
}

