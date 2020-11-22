#include <algorithm>
#include <vector>
#include "Graham_scan_alg.h"
#include "VPoint.h"

Graham_scan::Graham_scan() {  }

Vertices_double* Graham_scan::get_convex_hull(Vertices_double* points_)
{
    Vertices_double* convex_hull = new Vertices_double();
    Vertices_double* points      = new Vertices_double();

    for (auto point=points_->begin(); point != points_->end(); point++) {   // copy points to new vector
        points->push_back(*point);
    }

    VPoint<double>* most_left_point  = *points->begin();

    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        if ( (*point)->x < most_left_point->x ) { most_left_point = *point; }
    }

    points->erase(remove(points->begin(), points->end(), most_left_point));
    points->insert(points->begin(), 1, most_left_point);

    Vertices_double::iterator j;
    for (Vertices_double::iterator point=points->begin()+1; point != points->end(); point++) {
        j = point;
        while (j != points->begin()+1 && point_is_left_from_line(*j, *points->begin(), *(j-1)) < 0) {
            std::swap(*j, *(j-1));
            j -= 1;
        }
    }

    convex_hull->push_back(points->at(0));
    convex_hull->push_back(points->at(1));

    for (Vertices_double::iterator point=points->begin()+2; point != points->end(); point++) {
        while ( point_is_left_from_line(*point, *(convex_hull->end()-2), *(convex_hull->end()-1)) < 0) {
            convex_hull->pop_back();
        }
        convex_hull->push_back(*point);
    }
    return convex_hull;
}

template <typename T>
double Graham_scan::point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C){
    return (static_cast<double>(C->x) - B->x)*(A->y - B->y) - (C->y - B->y)*(A->x - B->x); }

