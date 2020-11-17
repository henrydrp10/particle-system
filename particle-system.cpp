////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2012-13
//
// Authors: Arturs Bekasovs and Toby Howard
//
/////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

// Display list for coordinate axis, and attributes
GLuint axisList;
int AXIS_SIZE= 200;
bool axisEnabled= false;

// Range of are where fireworks will be launched from
int LAUNCH_RANGE = 80;

// Number of particles in the array (they will be reused
// and recycled when dead so it seems that there are much more)
const int particleLength = 1;


double myRandom();

///////////////////////////////////////////////
// Class with the particle itself

class Particle {
public:

  // Color rgb -- starting black, fade to white
  GLfloat r, g, b;

  // Position of the particle
  GLfloat x , y, z;

  // Velocity of the particle
  GLfloat vx, vy, vz;

  // Acceleration of the particle
  // Glfloat ax, ay, az;

  // Initial size
  GLfloat size;

  // LifeLength
  GLint lifeLength;

};

Particle particles[particleLength];

///////////////////////////////////////////////
// Method to initialise a single particle of the array

void initParticle(int index) {

  // Initial color of the particle -- black at the moment
  particles[index].r = 0.0;
  particles[index].g = 0.0;
  particles[index].b = 0.0;

  // This defines the area of possible launch:
  // (-LAUNCH_RANGE/2, -LAUNCH_RANGE/2) to (LAUNCH_RANGE/2, LAUNCH_RANGE/2)
  particles[index].x = myRandom() * LAUNCH_RANGE - (LAUNCH_RANGE / 2);
  particles[index].y = 0.0;
  particles[index].z = myRandom() * LAUNCH_RANGE - (LAUNCH_RANGE / 2);

  // Initial velocity of the particle when launched
  particles[index].vx = 0;
  particles[index].vy = myRandom() * 5;
  particles[index].vz = 0;

  // Size, and time alive (if negative, time to be reborn)
  particles[index].size = 5;
  particles[index].lifeLength = 1;

}

///////////////////////////////////////////////
// Initialise all the particles in the array

void initParticles() {
  for (int index = 0; index < particleLength; index++) {
    initParticle(index);
  }
}

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////

void display()
{
  glLoadIdentity();

  // Position and direction of the camera
  gluLookAt(0.0, 0.0, 300.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);

  // Here, we will display the particle, modified according
  // to the translations and colour changes.
  for(int index = 0; index < particleLength; index ++) {
    if (particles[index].lifeLength >= 0) {
      glTranslatef(particles[index].x, particles[index].y, particles[index].z);
      glColor3f(particles[index].r, particles[index].g, particles[index].b);
      glutSolidSphere(particles[index].size, 150, 150);
    }
  }

  glutSwapBuffers();
}

///////////////////////////////////////////////
// TODO Add interactive functionality for the GUI

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(0);
      break;
    case 'a':
      axisEnabled = !axisEnabled;
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

///////////////////////////////////////////////

void reshape(int width, int height)
{
  glClearColor(0.9, 0.9, 0.9, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////
// Update the different values of the particle,
// according to the physics defined in this environment

void animations()
{



  glutPostRedisplay();

}

///////////////////////////////////////////////

void makeAxes() {
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
  glEndList();
}

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("COMP37111 Particles");
  initParticles();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(animations);
  makeAxes();
}

/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  // double f;
  srand(time(NULL));
  initGraphics(argc, argv);
  glEnable(GL_POINT_SMOOTH);
  glutMainLoop();
}
