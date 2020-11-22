#ifndef Voronoi_h
#define Voronoi_h

#include <list>
#include <queue>
#include <set>

#include "VPoint.h"
#include "VEdge.h"
#include "VParabola.h"
#include "VEvent.h"
#include <functional>


namespace vor
{
  /*
    Useful data containers for Vertices (places) and Edges of Voronoi diagram
  */
  typedef std::list<VPoint<double>*>  Vertices;
  typedef std::list<VEdge*>           Edges;

  /*
    Class for generating the Voronoi diagram
  */
  class Voronoi
  {
  public:

      Voronoi();

      /*
        input:
          v    : Vertices - places for drawing a diagram
          w    : width  of the result (top left corner is (0, 0))
          h    : height of the result

        output:
          pointer to list of edges

        All the data structures are managed by this class
      */
      Edges* get_edges(Vertices*, int, int);
  private:

      /*
              places        : container of places with which we work
              edges         : container of edges which will be then result
              width         : width of the diagram
              height        : height of the diagram
              root          : the root of the tree, that represents a beachline sequence
              line_y_cord   : current "y" position of the line (see Fortune's algorithm)
      */
      Vertices*   places;
      Edges*      edges;
      double      width, height;
      VParabola*  root;
      double      line_y_cord;

      /*
              deleted    : set of deleted (false) Events, since we can not delete from PriorityQueue
              points     : list of all new points that were created during the algorithm
              queue      : priority queue with events to process
      */
      std::set<VEvent*>  deleted;
      std::list<VPoint<double>*> points;
      std::priority_queue<VEvent*, std::vector<VEvent*>, VEvent::CompareEvent> queue;

      /*
              insert_parabola    : processing the place event
              remove_parabola    : processing the circle event
              finish_edge        : recursively finishes all infinite edges in the tree
              get_x_of_egde      : returns the current x position of an intersection point of left and right parabolas
              get_parabola_by_x  : returns the Parabola that is under this "x" position in the current beachline
              check_circle       : checks the circle event (disappearing) of this parabola
              GetEdgeInterse
      */
      void        insert_parabola(VPoint<double>*);
      void        remove_parabola(VEvent*);
      void        finish_edge(VParabola*);
      double      get_x_of_egde(VParabola*, double);
      VParabola*  get_parabola_by_x(double);
      double      get_y_of_point(VPoint<double>*, double);
      void        check_circle(VParabola*);
      VPoint<double>*     get_edge_intersection(VEdge*, VEdge*);
  };
}

#endif
