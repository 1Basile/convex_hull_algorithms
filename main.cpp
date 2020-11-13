#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>


#include "Voronoi.h"
#include "VPoint.h"

void display (void);
void rendering_func (int n);
void reshape (int width, int height);
void key_pressed(unsigned char key, int x, int y);
void special_key_pressed(int key, int x, int y);

vor::Voronoi*   voroni_alg;
vor::Vertices*  ver;
vor::Vertices*  dir; // points shift or speed depend on it`s starting position
vor::Edges*     edg;

double w = 10000000;
int NUMBER_OF_POINTS = 40;
int MOVING_SPEED = 30;
bool Delaunay_or_Voronoi = false;
bool ON_PAUSE = false;

int main (int argc, char **argv) 
{
    using namespace vor;

    voroni_alg = new Voronoi();
    ver = new Vertices();
    dir = new Vertices();

    srand(time(NULL)); // set random seed

    for(int i=0; i<NUMBER_OF_POINTS; i++) 
    {

     ver->push_back(new VPoint( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX )); 
     dir->push_back(new VPoint( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5)); 
    }

    edg = voroni_alg->get_edges(ver, w, w);

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode (GLUT_SINGLE); // Set up a basic display buffer (only single buffered for now)
    glutInitWindowSize (900, 900); // Set the width and height of the window
    glutInitWindowPosition (100, 100); // Set the position of the window
    glutCreateWindow ("Voronoi diagram"); // Set the title for the window

    glutTimerFunc(100, rendering_func, 0);
    glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering

    glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping

    glutKeyboardFunc(key_pressed); // Tell GLUT to use the method "key_pressed" for key presses
    glutSpecialFunc(special_key_pressed);

    glutMainLoop(); // Enter GLUT's main loop
    
    return 0;
}

void draw_voronoi_diagram()
{ if (!ON_PAUSE) { // Moving points
      vor::Vertices::iterator j = dir->begin();
      for(vor::Vertices::iterator i = ver->begin(); i != ver->end(); ++i)
      {
        (*i)->x += (*j)->x * w/50;
        (*i)->y += (*j)->y * w/50;
        if( (*i)->x > w ) (*j)->x *= -1;
        if( (*i)->x < 0 ) (*j)->x *= -1;

        if( (*i)->y > w ) (*j)->y *= -1;
        if( (*i)->y < 0 ) (*j)->y *= -1;
        ++j;
      }
                 }
 
 edg = voroni_alg->get_edges(ver, w, w);
 for(vor::Vertices::iterator i = ver->begin(); i!= ver->end(); ++i) // Draw points
 {
    glBegin(GL_QUADS);
      glVertex2f( -1+2*(*i)->x/w -0.01,  -1+2*(*i)->y/w -0.01);
      glVertex2f( -1+2*(*i)->x/w +0.01,  -1+2*(*i)->y/w -0.01);
      glVertex2f( -1+2*(*i)->x/w +0.01,  -1+2*(*i)->y/w +0.01);
      glVertex2f( -1+2*(*i)->x/w -0.01,  -1+2*(*i)->y/w +0.01);
    glEnd();
 }

 
 for(vor::Edges::iterator i = edg->begin(); i!= edg->end(); ++i) // Draw edges
 {
    if (Delaunay_or_Voronoi)
    {
        glBegin(GL_LINES); // Delaunay triangulation
        glVertex2f( -1+2*(*i)->left->x/w,  -1+2*(*i)->left->y/w);
        glVertex2f( -1+2*(*i)->right->x/w, -1+2*(*i)->right->y/w);
        glEnd();
    }
    else
    {
        glBegin(GL_LINES); // Voronoi diagram
        glVertex2f( -1+2*(*i)->start->x/w,  -1+2*(*i)->start->y/w);
        glVertex2f( -1+2*(*i)->end->x/w, -1+2*(*i)->end->y/w);
        glEnd();
    }

  }
}


void display (void) 
{
  glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
  glTranslatef(0.0f, 0.0f, -5.0f);

  glFlush();
}


void rendering_func(int n)
{
 glutTimerFunc(MOVING_SPEED, rendering_func, 0);
 glClear(GL_COLOR_BUFFER_BIT);  //Clear the screen
 glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // Clear the background of our window to red
 
 draw_voronoi_diagram();
 glutSwapBuffers(); //Draw everything to the screen
}


void reshape (int width, int height) 
{
 
 glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
 glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
 glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
 gluPerspective(22.5, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
 glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
}


void key_pressed(unsigned char key, int x, int y) {
  switch (key) {
      case 27: // ESC -> close
          glutDestroyWindow(glutGetWindow());
          exit(0);
          break;
      case 13: // ENTER -> switch mode
          Delaunay_or_Voronoi = !Delaunay_or_Voronoi;
          if (Delaunay_or_Voronoi) glutSetWindowTitle("Delaunay triangulation");
          else glutSetWindowTitle("Voronoi diagram");
          break;
      case 32: // Space -> pause executing
          ON_PAUSE = !ON_PAUSE;
          break;
      case 0x0065:  // GLUT_KEY_UP
          MOVING_SPEED += 4;
          std::cout << MOVING_SPEED << std::endl;
          break;
      case 0x0067:  //GLUT_KEY_DOWN
          MOVING_SPEED -= 4;
          std::cout << MOVING_SPEED << std::endl;
          break;

  }
}


void special_key_pressed(int key, int x, int y) {
  switch (key) {
      case 0x0067:  //GLUT_KEY_DOWN
          if (MOVING_SPEED >= 90) break;
          MOVING_SPEED += 4;
          break;
      case 0x0065:  // GLUT_KEY_UP
          if (MOVING_SPEED <= 20) break;
          MOVING_SPEED -= 4;
          break;
      case GLUT_KEY_RIGHT:
          if (ver->size() >= 100) break;
          ver->push_back(new VPoint( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX )); 
          dir->push_back(new VPoint( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5)); 
          edg = voroni_alg->get_edges(ver, w, w);
          break;
      case GLUT_KEY_LEFT:
          if (ver->size() <= 2) break;
          ver->pop_back(); 
          dir->pop_back(); 
          edg = voroni_alg->get_edges(ver, w, w);
          break;
  }
}
