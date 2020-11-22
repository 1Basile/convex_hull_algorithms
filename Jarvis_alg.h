#ifndef Jarvis_alg_h
#define Jarvis_alg_h

#include <iostream>
#include "VPoint.h"
#include <list>
#include <vector>

typedef std::vector<VPoint<double>*>  Vertices_double;

class Jarvis
{
    public:
      /* 
       Class for computing convex hull
      */
       Jarvis();

       /*
         input:
           vertices  : list of points to find convex hull

         output:
           vector, containing point that crate convex hull
        */

       Vertices_double* get_convex_hull(Vertices_double*);

    private:

      /*
          points            : container of points needed to be checked for convex hull
      */
      Vertices_double*     points;

      /*
          point_is_left_from_line  : return how much left is point A from BC line, 
                                     negative means point is right from line.
      */
      template <typename T>
      double        point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C);




};


#endif
