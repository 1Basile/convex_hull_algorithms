#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "Jarvis_alg.h"
#include "VPoint.h"

Jarvis::Jarvis() {  }

Vertices* Jarvis::get_convex_hull(Vertices* points_)
{
    Vertices* convex_hull = new Vertices();
    points = points_;
    // VPoint<double>* most_right = *points->begin();
    VPoint<double>* most_right_at_iteration;
    VPoint<double>* most_left_point  = *points->begin();

    for (Vertices::iterator point=points->begin(); point != points->end(); ++point) { 
        if ( (*point)->x < most_left_point->x ) { most_left_point = *point; }
    }
    convex_hull->push_back(most_left_point);
    remove(points->begin(), points->end(), most_left_point);
    points->push_back(most_left_point);

    while (true) {
        most_right_at_iteration = *points->begin();
        for (Vertices::iterator point=points->begin()+1; point != points->end(); ++point) 
        {
            if (point_is_left_from_line<double>(*point, most_right_at_iteration, *convex_hull->end()) < 0) { most_right_at_iteration = *point; } 
        }

        if (most_right_at_iteration == *(convex_hull->begin())) {break;std::cout << "end";}
        else { convex_hull->push_back(most_right_at_iteration); remove(points->begin(), points->end(), most_right_at_iteration); std::cout << "END__ONE";}
    
        }
    // for (Vertices::iterator point=points->begin(); point != points->end(); ++point) {
    //     if ( (*point)->x > most_right->x ) { most_right = *point; }
    // }

    // for (Vertices::iterator point=points->begin(); point != points->end(); ++point) {
    //     for (Vertices::iterator point=points->begin(); point != points->end(); ++point) {
    // }}


    for (Vertices::iterator point=points->begin(); point != points->end(); ++point) {  
      std::cout << "( "<< (*point)->x << ", "<< (*point)->y << ") | " << std::endl;
    }
    std::cout << "Max/Min points" << std::endl;
    // std::cout << "( "<< (most_right)->x << ", "<< (most_right)->y << ") | " << std::endl;
    std::cout << "( "<< (most_left_point)->x << ", "<< (most_left_point)->y << ") | " << std::endl;
    return convex_hull;
}
template <typename T>
double Jarvis::point_is_left_from_line(VPoint<T>* A, VPoint<T>* B, VPoint<T>* C){
    return (static_cast<double>(C->x) - B->x)*(A->y - B->y) - (C->y - B->y)*(A->x - B->x); }

