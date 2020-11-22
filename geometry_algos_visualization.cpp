#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <string>
#include <algorithm>
#include <ctime>
#include <map>

#include "Voronoi.h"
#include "VPoint.h"
#include "Kirkpatrick_alg.h"
#include "Jarvis_alg.h"
#include "Graham_scan_alg.h"
#include "Fast_convex_hull_alg.h"

void display ();
void rendering_func (int n);
void reshape (int width, int height);
void key_pressed(unsigned char key, int x, int y);
void special_key_pressed(int key, int x, int y);

vor::Voronoi*     voroni_alg;
Kirkpatrick*      kripatric_alg;
Jarvis*           jarvis_alg;
Graham_scan*      graham_scan_alg;
Fast_convex_hull* quickhull_alg;

vor::Vertices*  ver;
vor::Vertices*  dir; // points shift or speed depend on it`s starting position
vor::Edges*     edg;

Vertices_int*  ver_int;
Vertices_int*  dir_int;
Vertices_int*  convex_hull_int;

Vertices_double*  ver_double;
Vertices_double*  dir_double;
Vertices_double*  convex_hull_double;

std::map<int, std::string> method_name = {
                 {0, "Kirkpatrick convex hull"},
                 {1, "Delaunay triangulation"},
                 {2, "Gift wrapping algorithm"},
                 {3, "Voronoi diagram"},
                 {4, "Graham scan algorithm"},
                 {5, "Quickhull algorithm"} };

enum Algorithms {KIRKPATRICK_ALGO, DELAUNAY_ALGO, JARVIS_ALGO, VORONI_ALGO, GRAHAMS_ALGO, QUICKHULL_ALGO};
Algorithms current_method_num = KIRKPATRICK_ALGO;

double w             = 10000000;
int w_int            = 900;
int NUMBER_OF_POINTS = 40;
int MOVING_SPEED     = 30;
bool ON_PAUSE        = false;

int main (int argc, char **argv) 
{
    using namespace vor;

    voroni_alg = new Voronoi();
    kripatric_alg = new Kirkpatrick();

    ver = new vor::Vertices();
    dir = new vor::Vertices();

    ver_int = new Vertices_int();
    dir_int = new Vertices_int();

    ver_double = new Vertices_double();
    dir_double = new Vertices_double();

    srand(time(nullptr)); // set random seed

    for(int i=0; i<NUMBER_OF_POINTS; i++) 
    {

     ver->push_back(new VPoint<double>( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX ));
     dir->push_back(new VPoint<double>( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5));

     ver_int->push_back(new VPoint<int>( rand() % w_int, rand() % w_int ));
     dir_int->push_back(new VPoint<int>( rand()%w_int - w_int/2, rand()%w_int - w_int/2));

     ver_double->push_back(new VPoint<double>( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX ));
     dir_double->push_back(new VPoint<double>( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5));

    }

    edg = voroni_alg->get_edges(ver, w, w);

    convex_hull_double = jarvis_alg->get_convex_hull(ver_double);

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode (GLUT_SINGLE); // Set up a basic display buffer (only single buffered for now)
    glutInitWindowSize (900, 900); // Set the width and height of the window
    glutInitWindowPosition (100, 100); // Set the position of the window
    glutCreateWindow (method_name.at(current_method_num).c_str()); // Set the title for the window

    glutTimerFunc(100, rendering_func, 0);
    glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering

    glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping

    glutKeyboardFunc(key_pressed); // Tell GLUT to use the method "key_pressed" for key presses
    glutSpecialFunc(special_key_pressed);

    glutMainLoop(); // Enter GLUT's main loop
    
    return 0;
}

void draw_voronoi_delone_diagram()
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
    if (current_method_num == DELAUNAY_ALGO)
    {
        glBegin(GL_LINES); // Delaunay triangulation
        glVertex2f( -1+2*(*i)->left->x/w,  -1+2*(*i)->left->y/w);
        glVertex2f( -1+2*(*i)->right->x/w, -1+2*(*i)->right->y/w);
        glEnd();
    }
    else if (current_method_num == VORONI_ALGO)
    {
        glBegin(GL_LINES); // Voronoi diagram
        glVertex2f( -1+2*(*i)->start->x/w,  -1+2*(*i)->start->y/w);
        glVertex2f( -1+2*(*i)->end->x/w, -1+2*(*i)->end->y/w);
        glEnd();
    }

  }
}
void draw_convex_hull_double()

{ if (!ON_PAUSE) { // Moving points
        Vertices_double::iterator j = dir_double->begin();
        for(Vertices_double::iterator i = ver_double->begin(); i != ver_double->end(); ++i)
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

    switch (current_method_num) {
        case JARVIS_ALGO:
            convex_hull_double = jarvis_alg->get_convex_hull(ver_double);
            break;
        case QUICKHULL_ALGO:
            convex_hull_double = quickhull_alg->get_convex_hull(ver_double);
            break;
        case GRAHAMS_ALGO:
            convex_hull_double = graham_scan_alg->get_convex_hull(ver_double);
            break;
    }

    for(Vertices_double::iterator i = ver_double->begin(); i!= ver_double->end(); ++i) // Draw points
    {
        glBegin(GL_QUADS);
        glVertex2f( -1+2*(*i)->x/w -0.01,  -1+2*(*i)->y/w -0.01);
        glVertex2f( -1+2*(*i)->x/w +0.01,  -1+2*(*i)->y/w -0.01);
        glVertex2f( -1+2*(*i)->x/w +0.01,  -1+2*(*i)->y/w +0.01);
        glVertex2f( -1+2*(*i)->x/w -0.01,  -1+2*(*i)->y/w +0.01);
        glEnd();
    }


    glColor3f(232.0/255,168.0/255,124.0/255);
    glBegin(GL_POLYGON);
    glClearColor(0.5f, 0.0f, 0.2f, 1.0f); // clear the background of our window to red
    for(Vertices_double::iterator j = convex_hull_double->begin(); j!= convex_hull_double->end(); ++j)
    {
        glVertex2f(-1+2*(double)(*j)->x/w, -1+2*(double)(*j)->y/w);
    }
    glEnd();

    for(Vertices_double::iterator i = ver_double->begin(); i!= ver_double->end(); ++i) // draw points
    {
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-1+2*(double)(*i)->x/w-0.01, -1+2*(double)(*i)->y/w-0.01);
        glVertex2f(-1+2*(double)(*i)->x/w+0.01, -1+2*(double)(*i)->y/w-0.01);
        glVertex2f(-1+2*(double)(*i)->x/w+0.01, -1+2*(double)(*i)->y/w+0.01);
        glVertex2f(-1+2*(double)(*i)->x/w-0.01, -1+2*(double)(*i)->y/w+0.01);
        glEnd();
    }
}
void draw_convex_hull_int()
{ 
      if (!ON_PAUSE) { // Moving points
          Vertices_int::iterator j = dir_int->begin();
          for(Vertices_int::iterator i = ver_int->begin(); i != ver_int->end(); ++i)
          {
            (*i)->x += 4*(*j)->x/abs((*j)->x);
            (*i)->y += 4*(*j)->y/abs((*j)->y);

            if( (*i)->x > w_int ) (*j)->x *= -1;
            if( (*i)->x < 0 )   (*j)->x *= -1;

            if( (*i)->y > 900 ) (*j)->y *= -1;
            if( (*i)->y < 0 )   (*j)->y *= -1;
            ++j;
          }
                   }
     convex_hull_int = kripatric_alg->get_convex_hull(ver_int);

     glColor3f(232.0/255,168.0/255,124.0/255);
     glBegin(GL_POLYGON);
     glClearColor(0.5f, 0.0f, 0.2f, 1.0f); // clear the background of our window to red
     for(Vertices_int::iterator j = convex_hull_int->begin(); j!= convex_hull_int->end(); ++j)
     {
          glVertex2f( -1+2*(double)(*j)->x/w_int, -1+2*(double)(*j)->y/w_int);
     }
     glEnd();

     for(Vertices_int::iterator i = ver_int->begin(); i!= ver_int->end(); ++i) // draw points
     {
          glColor3f(1.0f,1.0f,1.0f);
          glBegin(GL_QUADS);
                   glVertex2f(-1+2*(double)(*i)->x/w_int-0.01, -1+2*(double)(*i)->y/w_int-0.01);
                   glVertex2f(-1+2*(double)(*i)->x/w_int+0.01, -1+2*(double)(*i)->y/w_int-0.01);
                   glVertex2f(-1+2*(double)(*i)->x/w_int+0.01, -1+2*(double)(*i)->y/w_int+0.01);
                   glVertex2f(-1+2*(double)(*i)->x/w_int-0.01, -1+2*(double)(*i)->y/w_int+0.01);
           glEnd();
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
 
 switch (current_method_num) {
    case VORONI_ALGO:
       draw_voronoi_delone_diagram();
       break;
    case DELAUNAY_ALGO:
       draw_voronoi_delone_diagram();
       break;
    case KIRKPATRICK_ALGO:
       draw_convex_hull_int();
       break;
    case GRAHAMS_ALGO:
       draw_convex_hull_double();
       break;
    case JARVIS_ALGO:
       draw_convex_hull_double();
       break;
    case QUICKHULL_ALGO:
        draw_convex_hull_double();
        break;
 }
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
      case 13: // ENTER -> switch mode
          current_method_num = static_cast<Algorithms>((current_method_num + 1) % method_name.size());
          glutSetWindowTitle(method_name.at(current_method_num).c_str());
          break;
      case 32: // Space -> pause executing
          ON_PAUSE = !ON_PAUSE;
          break;
  }
}


void special_key_pressed(int key, int x, int y) {
  switch (key) {
      case GLUT_KEY_DOWN:
          if (MOVING_SPEED >= 90) break;
          MOVING_SPEED += 4;
          break;
      case GLUT_KEY_UP:
          if (MOVING_SPEED <= 20) break;
          MOVING_SPEED -= 4;
          break;
      case GLUT_KEY_RIGHT:
          if (ver->size() >= 100) break;
          ver->push_back(new VPoint<double>( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX )); 
          dir->push_back(new VPoint<double>( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5));

          ver_int->push_back(new VPoint<int>( rand() % w_int, rand() % w_int ));
          dir_int->push_back(new VPoint<int>( rand()%w_int - w_int/2, rand()%w_int - w_int/2));

          ver_double->push_back(new VPoint<double>( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX ));
          dir_double->push_back(new VPoint<double>( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5));

          edg                = voroni_alg->get_edges(ver, w, w);
          convex_hull_int    = kripatric_alg->get_convex_hull(ver_int);
          convex_hull_double = jarvis_alg->get_convex_hull(ver_double);
          break;
      case GLUT_KEY_LEFT:
          if (ver->size() <= 3) break;
          ver->pop_back(); 
          dir->pop_back();

          ver_int->pop_back(); 
          dir_int->pop_back();

          ver_double->pop_back();
          dir_double->pop_back();

          edg = voroni_alg->get_edges(ver, w, w);
          convex_hull_int = kripatric_alg->get_convex_hull(ver_int);
          convex_hull_double = jarvis_alg->get_convex_hull(ver_double);
          break;
  }
}
