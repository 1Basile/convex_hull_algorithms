#ifndef VEdge_h
#define VEdge_h

#include "VPoint.h"
#include <iostream>

/*
  A class that stores an edge in Voronoi diagram

  start      : pointer to start point
  end        : pointer to end point
  left       : pointer to Voronoi place on the left side of edge
  right      : pointer to Voronoi place on the right side of edge

  neighbour  : some edges consist of two parts, so we add the pointer to another part to connect them at the end of an algorithm

  direction  : directional vector, from "start", points to "end", normal of |left, right|
  a, b       : directional coeffitients satisfying equation y = a*x + b (edge lies on this line)
*/

class VEdge
{
public:

  VPoint<double>*  start;
  VPoint<double>*  end;
  VPoint<double>*  direction;
  VPoint<double>*  left;
  VPoint<double>*  right;

  double   a;
  double   b;

  VEdge* neighbour;

  /*
    Constructor of the class

    s    : pointer to start
    a    : pointer to left place
    b    : pointer to right place
  */
  VEdge(VPoint<double>* s, VPoint<double>* l, VPoint<double>* r)
  {
    start      = s;
    end        = nullptr;
    left       = l;
    right      = r;
    neighbour  = nullptr;

    a = (r->x - l->x) / (l->y - r->y) ;
    b = s->y - a * s->x ;
    direction = new VPoint<double>(r->y - l->y, -(r->x - l->x));
  }

  /*
    Destructor of the class
    direction belongs only to the current edge, other pointers can be shared by other edges
  */
  ~VEdge()
  {
    delete direction;
  }

};

#endif
