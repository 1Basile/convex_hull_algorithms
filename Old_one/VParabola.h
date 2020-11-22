#ifndef VParabola_h
#define VParabola_h

#include "VPoint.h"
#include "VEdge.h"

/*
  Parabolas and events have pointers to each other.
*/

class VEvent;

/*
  A class that stores information about an item in beachline sequence.
  It can represent an arch of parabola or an intersection between two archs (which defines an edge).
  Made with a binary tree (nodes are edges, leaves are archs).
*/

class VParabola
{
public:

  /*
    isLeaf    : flag whether the node is Leaf or internal node
    site      : pointer to the focus point of parabola (when it is parabola)
    edge      : pointer to the edge (when it is an edge)
    cEvent    : pointer to the event, when the arch disappears (circle event)
    parent    : pointer to the parent node in tree
  */
  VPoint<double>*   site;
  bool              isLeaf;
  VEdge*            edge;
  VEvent*           cEvent;
  VParabola*        parent;

  /*
    Constructors (empty for edge, with focus parameter for an arch).
  */
  VParabola  (VPoint<double>* s)
{
  site    = s;
  isLeaf  = true;
  cEvent  = 0;
  edge    = 0;
  parent  = 0;
}
  VParabola  ();

  /*
    Access to the children (in tree).
  */
  void    SetLeft  (VParabola* p) {_left  = p; p->parent = this;}
  void    SetRight (VParabola* p) {_right = p; p->parent = this;}

  VParabola*  Left () { return _left;  }
  VParabola*  Right() { return _right; }

  /*
    GetLeft        : returns the closest left leave of the tree
    GetRight       : returns the closest right leafe of the tree
    GetLeftParent  : returns the closest parent which is on the left
    GetLeftParent  : returns the closest parent which is on the right
    GetLeftChild   : returns the closest leave which is on the left of current node
    GetRightChild  : returns the closest leave which is on the right of current node
  */
  static VParabola* GetLeft         (VParabola* p);
  static VParabola* GetRight        (VParabola* p);
  static VParabola* GetLeftParent   (VParabola* p);
  static VParabola* GetRightParent  (VParabola* p);
  static VParabola* GetLeftChild    (VParabola* p);
  static VParabola* GetRightChild   (VParabola* p);

private:
  VParabola* _left;
  VParabola* _right;
};

#endif