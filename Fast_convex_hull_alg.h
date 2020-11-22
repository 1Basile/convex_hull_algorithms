#ifndef Fast_convex_hull_alg_h
#define Fast_convex_hull_alg_h

#include <iostream>
#include "VPoint.h"
#include <list>
#include <vector>

typedef std::vector<VPoint<double>*>  Vertices_double;

class Fast_convex_hull
{
public:
    /*
     Class for computing convex hull
    */
    Fast_convex_hull();

    /*
      input:
        vertices  : list of points to find convex hull

      output:
        list, containing point that crate convex hull
     */

    Vertices_double* get_convex_hull(Vertices_double*);

private:

    /*
        points            : container of points needed to be checked for convex hull
        convex_hull       : container of points for convex hull
    */
    Vertices_double*     points{};
    Vertices_double*     convex_hull = new Vertices_double();

    /*
        point_is_left_from_line  : return how much left is point A from BC line,
                                   negative means point is right from line.
        recursive_convex_hull    : function find most left and right points, build max triangle
                                  and return points out of it.
    */
    void recursive_convex_hull(Vertices_double*, VPoint<double>*, VPoint<double>*, Vertices_double*);

    template <typename T>
    double        point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C);

    static double distance_from_point(VPoint<double> *pPoint, VPoint<double> *pPoint1, VPoint<double> *&pPoint2);

//    void recursive_convex_hull(Vertices_double *points_, VPoint<double> *A, VPoint<double> *B);
};

#endif
