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

#define NORTH_VIEW 1
#define SOUTH_VIEW 2
#define EAST_VIEW 3
#define WEST_VIEW 4

// Current view
int view = NORTH_VIEW;

// Display list for coordinate axis, and attributes
GLuint axisList;
int AXIS_SIZE= 200;
bool axisEnabled= true;

// Range of are where fireworks will be launched from
int LAUNCH_RANGE = 80;

// Number of main particles in the array (they will be reused
// and recycled when dead so it seems that there are much more)
const int particleLength = 20;

// 10 explosion particles per main particle
const int explosionPartLength = particleLength * 10;

// Forces that will act as acceleration in multiple dimensions
GLfloat gravity = -0.01;

// Wind that will affect the direction of the particles
GLfloat xWind = 0;
GLfloat zWind = 0;


double myRandom();

///////////////////////////////////////////////
// Class with the particle itself

class Particle {
public:

  // Color rgb -- starting black, fade to white
  GLfloat r, g, b, alpha;

  // Position of the particle
  GLfloat x , y, z;

  // Old Position of the particle
  GLfloat ox , oy, oz;

  // Velocity of the particle
  GLfloat vx, vy, vz;

  // Initial size
  GLfloat size;

  // LifeLength
  GLint lifeLength;

};

Particle particles[particleLength];
Particle explosionParticles[explosionPartLength];

///////////////////////////////////////////////
// Method to initialise a single particle of the array

void initParticle(int index) {

  // Initial color of the particle -- black at the moment
  particles[index].r = 0.0;
  particles[index].g = 0.0;
  particles[index].b = 0.0;
  particles[index].alpha = 1.0;

  // This defines the area of possible launch:
  // (-LAUNCH_RANGE/2, -LAUNCH_RANGE/2) to (LAUNCH_RANGE/2, LAUNCH_RANGE/2)
  particles[index].x = myRandom() * LAUNCH_RANGE - (LAUNCH_RANGE / 2);
  particles[index].y = 0.0;
  particles[index].z = myRandom() * LAUNCH_RANGE - (LAUNCH_RANGE / 2);

  particles[index].ox = myRandom() * LAUNCH_RANGE - (LAUNCH_RANGE / 2);
  particles[index].oy = 0.0;
  particles[index].oz = myRandom() * LAUNCH_RANGE - (LAUNCH_RANGE / 2);

  // printf("(%f, %f)\n", particles[index].x, particles[index].z);

  // Initial velocity of the particle when launched
  particles[index].vx = myRandom() * 0.1 - 0.05;
  particles[index].vy = myRandom() * 0.3 + 0.7;
  particles[index].vz = myRandom() * 0.1 - 0.05;

  // printf("%f\n", particles[index].vy);

  // Size, and time alive (if negative, time to be reborn)
  particles[index].size = 1;
  particles[index].lifeLength = 0;

}

///////////////////////////////////////////////
// Method to initialise the explosionParticles

void initExplosionParticles(int index) {

  for (int i = index * 10; i < index * 10 + 10; i++) {

    // Initial color of the particle -- red at the moment
    explosionParticles[i].r = 1.0;
    explosionParticles[i].g = 0.0;
    explosionParticles[i].b = 0.0;
    explosionParticles[i].alpha = 1.0;

    // Starting Position (where main particle died)
    explosionParticles[i].x = particles[index].x;
    explosionParticles[i].y = particles[index].y;
    explosionParticles[i].z = particles[index].z;

    explosionParticles[i].ox = particles[index].x;
    explosionParticles[i].oy = particles[index].y;
    explosionParticles[i].oz = particles[index].z;

    // Initial velocity of the particle when exploded
    explosionParticles[i].vx = (myRandom() * 0.1 - 0.05) * 5;
    explosionParticles[i].vy = (myRandom() * 0.3 + 0.7) / 1.5;
    explosionParticles[i].vz = (myRandom() * 0.1 - 0.05) * 5;


    // Size, and time alive (if negative, time to be reborn)
    explosionParticles[i].size = 4;
    explosionParticles[i].lifeLength = 60;

  }

}

///////////////////////////////////////////////
// Method to update the explosionParticles

void updateExplosionParticles(int index) {

  for (int i = index * 10; i < index * 10 + 10; i++) {

    explosionParticles[i].lifeLength--;

    if (explosionParticles[i].lifeLength >= 0) {

      explosionParticles[i].alpha -= 0.01;

      explosionParticles[i].ox = explosionParticles[i].x;
      explosionParticles[i].oy = explosionParticles[i].y;
      explosionParticles[i].oz = explosionParticles[i].z;

      explosionParticles[i].x += explosionParticles[i].vx;
      explosionParticles[i].y += explosionParticles[i].vy;
      explosionParticles[i].z += explosionParticles[i].vz;

      explosionParticles[i].vx += xWind;
      explosionParticles[i].vy += gravity;
      explosionParticles[i].vz += zWind;

    }


  }

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
// Initialise all the particles in the array

void drawFloorPlane() {

  // Draw the blue floor
  glBegin(GL_POLYGON);
    glColor3f(0.3, 1.0, 1.0);
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(-40.0, 0.0,  40.0);
    glVertex3f( 40.0, 0.0,  40.0);
    glVertex3f( 40.0, 0.0, -40.0);
  glEnd();

  // Draw the lines
  for (int index = -30; index < 40; index += 10) {
    glBegin(GL_LINES);
      glColor3f(0.5, 0.5, 0.5);
      glVertex3f(index, 0, -40);
      glVertex3f(index, 0, 40);
      glVertex3f(-40, 0, index);
      glVertex3f(40, 0, index);
    glEnd();
  }

}

///////////////////////////////////////////////
// Initialise all the particles in the array

void setView() {


  glLoadIdentity();

  switch (view) {
    case NORTH_VIEW:
      gluLookAt(0.0, 30.0, 60.0,
                0.0, 30.0, 0.0,
                0.0, 1.0, 0.0);
      break;
    case SOUTH_VIEW:
      gluLookAt(0.0, 30.0, -60.0,
                0.0, 30.0, 0.0,
                0.0, 1.0, 0.0);
      break;
    case EAST_VIEW:
      gluLookAt(60.0, 30.0, 0.0,
                0.0, 30.0, 0.0,
                0.0, 1.0, 0.0);
      break;
    case WEST_VIEW:
      gluLookAt(-60.0, 30.0, 0.0,
                0.0, 30.0, 0.0,
                0.0, 1.0, 0.0);
      break;
  }

}

///////////////////////////////////////////////

void display()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the view
  setView();

  // Draw the floor (launch area)
  drawFloorPlane();

  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);

  // Here, we will display the particle, modified according
  // to the translations and colour changes.
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  for(int index = 0; index < particleLength; index ++) {
    if (particles[index].lifeLength >= 0) {
      glPushMatrix();
      glTranslatef(particles[index].x, particles[index].y, particles[index].z);
      glColor4f(particles[index].r, particles[index].g, particles[index].b, particles[index].alpha);
      glutSolidSphere(particles[index].size, 150, 150);
      glPopMatrix();
    } else {
      glBegin(GL_LINES);
      for (int i = index * 10; i < index * 10 + 10; i++) {
        if (explosionParticles[i].lifeLength >= 0 && explosionParticles[i].alpha > 0) {
          glColor4f(explosionParticles[i].r, explosionParticles[i].g, explosionParticles[i].b, explosionParticles[i].alpha);
          glVertex3f(explosionParticles[i].ox, explosionParticles[i].oy, explosionParticles[i].oz);
          glVertex3f(explosionParticles[i].x, explosionParticles[i].y, explosionParticles[i].z);
        }

      }
      glEnd();
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

  // Loop through all particles
  for (int index = 0; index < particleLength; index++) {

    // Time passes
    particles[index].lifeLength++;

    // If time to be born
    if (particles[index].lifeLength == 0) {
      initParticle(index);

      // If alive
    } else if (particles[index].lifeLength > 0) {

      // Updating size, position and velocity

      particles[index].size -= 0.01;
      particles[index].alpha -= 0.01;

      particles[index].x += particles[index].vx;
      particles[index].y += particles[index].vy;
      particles[index].z += particles[index].vz;

      particles[index].vx += xWind;
      particles[index].vy += gravity;
      particles[index].vz += zWind;

      // Time to die
      if (particles[index].vy < 0) {
        particles[index].lifeLength = myRandom() * 100 - 100;

        initExplosionParticles(index);
      }

    } else {

      updateExplosionParticles(index);


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
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
  glutCreateWindow("COMP37111 Particles");
  initParticles();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(animations);
  makeAxes();
}

///////////////////////////////////////////////
void menu (int menuentry) {
  switch (menuentry) {
  case 1: gravity = -0.007;
          break;
  case 2: gravity = -0.01;
          break;
  case 3: gravity = -0.013;
          break;
  case 4: xWind = 0.004;
          zWind = 0;
          break;
  case 5: xWind = -0.004;
          zWind = 0;
          break;
  case 6: xWind = 0;
          zWind = -0.004;
          break;
  case 7: xWind = 0;
          zWind = 0.004;
          break;
  case 8: view = NORTH_VIEW;
          break;
  case 9: view = SOUTH_VIEW;
          break;
  case 10: view = EAST_VIEW;
          break;
  case 11: view = WEST_VIEW;
          break;
  case 12: exit(0);
  }
} // menu() {


///////////////////////////////////////////////
void initMenu()
{
  glutCreateMenu (menu);
  glutAddMenuEntry ("Low gravity", 1);
  glutAddMenuEntry ("Medium gravity", 2);
  glutAddMenuEntry ("High gravity", 3);
  glutAddMenuEntry ("West wind", 4);
  glutAddMenuEntry ("East wind", 5);
  glutAddMenuEntry ("North wind", 6);
  glutAddMenuEntry ("South wind", 7);
  glutAddMenuEntry ("North view", 8);
  glutAddMenuEntry ("South view", 9);
  glutAddMenuEntry ("East view", 10);
  glutAddMenuEntry ("West view", 11);
  glutAddMenuEntry ("Exit", 12);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

}

/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  // double f;
  srand(time(NULL));
  initGraphics(argc, argv);
  initMenu();
  glEnable(GL_POINT_SMOOTH);
  glutMainLoop();
}
