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

// Display list for coordinate axis
GLuint axisList;

int AXIS_SIZE= 200;
bool axisEnabled= true;
int launch_range = 80;
const int particleLength = 50;

double myRandom();

///////////////////////////////////////////////

class Particle {


public:

  // Color rgb -- starting black, fade to white
  GLfloat r, g, b;

  // Initial Position
  GLfloat x , y, z;

  // Initial velocity
  GLfloat vx, vy, vz;

  // Initial size
  GLfloat size;

  // LifeLength
  GLint lifeLength;

};

Particle particles[particleLength];

///////////////////////////////////////////////

void initParticle(int index) {

  particles[index].r = 1.0;

  particles[index].x = myRandom() * launch_range - (launch_range / 2);
  particles[index].y = 0.0;
  particles[index].z = myRandom() * launch_range - (launch_range / 2);

  particles[index].vx = myRandom() * 10;
  particles[index].vy = myRandom() * 10;
  particles[index].vz = myRandom() * 10;

  particles[index].size = 5;
  particles[index].lifeLength = 1;

}

///////////////////////////////////////////////

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
  gluLookAt(0.0, 800.0, 800.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);

  // Here, we will put the translations according
  // to the physics of the game.
  for(int index = 0; index < particleLength; index ++) {
    if (particles[index].lifeLength >= 0) {
      glTranslatef(particles[index].x, particles[index].y, particles[index].z);
      glColor3f(particles[index].r, 0.0, 0.0);
      glutSolidSphere(particles[index].size, 150, 150);
    }
  }


  glutSwapBuffers();
}

///////////////////////////////////////////////

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

void animations()
{
  // 1.- Remove any particles past lifetime.
  // 2.- Create new particles and initialise them.
  // 3.- Update attributes such as pos., veloc. and accel.
  // (First check position -- COLLISIONS
  //  Then colour, velocity, etc);
  for (int index = 0; index < particleLength; index++) {

    particles[index].lifeLength++;

    if (particles[index].lifeLength > 0) {
      particles[index].size -= 0.05;
      particles[index].r -= 0.01;

      particles[index].x += particles[index].vx - 2;
      particles[index].y = particles[index].vy - 9.8 / 2;
      particles[index].z += particles[index].vz - 2;

      particles[index].vx -= 2;
      particles[index].vy -= 9.8;
      particles[index].vz -= 2;

      if (particles[index].y < 0) {
        particles[index].vy /= -2;
        particles[index].y = 0;
      }

      if (particles[index].vx < 0) particles[index].vx = 0;
      if (particles[index].vy < 0) particles[index].vy = 0;
      if (particles[index].vz < 0) particles[index].vz = 0;

      if (particles[index].vx == 0 && particles[index].vy == 0 &&
          particles[index].vz == 0) {
        particles[index].lifeLength = -100;
      }

    } else if (particles[index].lifeLength == 0) {
      initParticle(index);
    }

  }


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
