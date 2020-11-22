#include <algorithm>
#include <vector>
#include "Jarvis_alg.h"
#include "VPoint.h"

Jarvis::Jarvis() {  }

Vertices_double* Jarvis::get_convex_hull(Vertices_double* points_)
{
    Vertices_double* convex_hull = new Vertices_double();
    Vertices_double*     points = new Vertices_double();
    for (auto point=points_->begin(); point != points_->end(); point++) {
        points->push_back(*point);
    }
    // VPoint<double>* most_right = *points->begin();
    VPoint<double>* most_right_at_iteration;
    VPoint<double>* most_left_point  = *points->begin();
    for (Vertices_double::iterator point=points->begin(); point != points->end(); ++point) {
        if ( (*point)->x < most_left_point->x ) { most_left_point = *point; }
    }


    convex_hull->push_back(most_left_point);
    points->erase(remove(points->begin(), points->end(), most_left_point));
    points->push_back(most_left_point);

    while (true) {
        most_right_at_iteration = *points->begin();
        for (Vertices_double::iterator point=points->begin()+1; point != points->end(); ++point)
        {
            if (point_is_left_from_line<double>(*point, most_right_at_iteration, *(convex_hull->end()-1)) < 0) { most_right_at_iteration = *point; }
        }

        if (most_right_at_iteration == *(convex_hull->begin())) {break;}
        else { convex_hull->push_back(most_right_at_iteration); remove(points->begin(), points->end(), most_right_at_iteration);}
    
        }

    return convex_hull;
}
template <typename T>
double Jarvis::point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C){
    return (static_cast<double>(C->x) - B->x)*(A->y - B->y) - (C->y - B->y)*(A->x - B->x); }