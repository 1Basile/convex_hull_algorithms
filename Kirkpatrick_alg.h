#ifndef Kirkpatrick_alg_h
#define Kirkpatrick_alg_h

#include <iostream>
#include "VPoint.h"
#include <list>
#include <vector>

typedef std::vector<VPoint<int>*>  Vertices_int;

class Kirkpatrick
{
    public:
      /* 
       Class for computing convex hull
      */
       Kirkpatrick();

       /*
         input:
           vertices  : list of points to find convex hull

         output:
           list, containing point that crate convex hull
        */

       Vertices_int* get_convex_hull(Vertices_int*);

    private:

      /*
          points            : container of points needed to be checked for convex hull
          most_left_points  : container with most left points for each y coordinate
          most_right_points : container with most right points for each y coordinate
      */
      Vertices_int*     most_right_points;
      Vertices_int*     most_left_points;
      Vertices_int*     points;

      /*
          sort_by_y_cordinate      : sort all points in list with ___ sort
          point_is_left_from_line  : return how much left is point A from BC line, 
                                     negative means point is right from line.
          sort_by_left_turn        : sort suspitious on convex hull points
          get_left_most_points     : return list of most left points for each y coordinate
          get_right_most_points    : return list of most right points for each y coordinate
      */
      void          sort_by_y_cordinate();
      template <typename T>
      double        point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C);
      Vertices_int*     get_left_most_points();
      Vertices_int*     get_right_most_points();
      Vertices_int*     sort_by_left_turn(Vertices_int*);
      Vertices_int*     sort_by_right_turn(Vertices_int*);




};


#endif
